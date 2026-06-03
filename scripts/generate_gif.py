#!/usr/bin/env python3
import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from datetime import datetime

import numpy as np
from PIL import Image
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.tri import Triangulation


def read_obj(filename):
    verts = []
    faces = []
    with open(filename) as f:
        for line in f:
            if line.startswith("v "):
                _, x, y, z = line.split()
                verts.append((float(x), float(y), float(z)))
            elif line.startswith("f "):
                parts = line.split()
                a, b, c = int(parts[1]), int(parts[2]), int(parts[3])
                faces.append((a - 1, b - 1, c - 1))
    return np.array(verts), np.array(faces)


def render_frame(verts, faces, azim, elev, title=""):
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection="3d")

    tri = Triangulation(verts[:, 0], verts[:, 1], faces)
    surf = ax.plot_trisurf(tri, verts[:, 2], cmap="viridis",
                           shade=True, alpha=0.95, linewidth=0,
                           antialiased=True)

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.view_init(elev=elev, azim=azim)
    if title:
        ax.set_title(title)

    fig.colorbar(surf, ax=ax, shrink=0.6, label="Altura z")
    fig.canvas.draw()
    w, h = fig.canvas.get_width_height()
    buf = np.frombuffer(fig.canvas.tostring_argb(), dtype=np.uint8)
    buf.shape = (h, w, 4)
    buf = buf[:, :, [1, 2, 3, 0]]
    plt.close(fig)
    return Image.fromarray(buf)


def main():
    p = argparse.ArgumentParser(
        description="Genera un GIF animado de abolladuras progresivas")

    p.add_argument("--out", default="anim.gif")
    p.add_argument("--fps", type=float, default=0.56)
    p.add_argument("--frames", type=int, default=20)
    p.add_argument("--i-max", type=float, default=30)
    p.add_argument("--rbf", default="gaussian")
    p.add_argument("--grid", nargs=2, type=int, default=[40, 30])
    p.add_argument("--azim", type=float, default=-60)
    p.add_argument("--elev", type=float, default=30)
    p.add_argument("--dents", type=int, default=3)
    p.add_argument("--r-range", nargs=2, type=float, default=[10, 30])
    p.add_argument("--bash", default="./build/bash_surface",
                    help="Path to bash_surface binary")
    p.add_argument("--surf-script",
                    default="./scripts/generate_surface.py",
                    help="Path to generate_surface.py")
    args = p.parse_args()

    tmp = tempfile.mkdtemp(prefix="gif_")
    base_obj = os.path.join(tmp, "base.obj")
    base_dents = os.path.join(tmp, "base_dents.txt")

    subprocess.run([
        sys.executable, args.surf_script,
        str(args.grid[0]), str(args.grid[1]), str(args.dents),
        "--flat", "--x-range", "0", "200", "--y-range", "0", "200",
        "--r-range", str(args.r_range[0]), str(args.r_range[1]),
        "--i-range", "0", str(args.i_max),
        "--out", os.path.join(tmp, "base")
    ], check=True)

    originals = os.listdir(tmp)
    for f in originals:
        if f.endswith(".obj") or f.endswith("_dents.txt"):
            pth = os.path.join(tmp, f)
            if "base_" in f:
                base_obj = pth if f.endswith(".obj") else base_obj
                base_dents = pth if f.endswith("_dents.txt") else base_dents

    n_dents = args.dents
    dents_data = []
    with open(base_dents) as fd:
        for line in fd:
            line = line.strip()
            if not line:
                continue
            center, radius, _ = line.split(";")
            dents_data.append((center, radius))

    images = []
    rng = np.random.default_rng(1)

    for frame in range(args.frames):
        t = frame / (args.frames - 1) if args.frames > 1 else 1
        I = t * args.i_max

        frame_dents = os.path.join(tmp, f"frame_dents_{frame:03d}.txt")
        with open(frame_dents, "w") as f:
            for center, radius in dents_data:
                f.write(f"{center};{radius};{I:.6f}\n")

        frame_obj = os.path.join(tmp, f"frame_{frame:03d}.obj")
        subprocess.run([
            args.bash, base_obj, frame_dents, args.rbf, frame_obj
        ], check=True, capture_output=True)

        verts, faces = read_obj(frame_obj)
        title = f"I = {I:.1f}  ({args.rbf})"
        im = render_frame(verts, faces, args.azim, args.elev, title)
        images.append(im)
        print(f"Frame {frame + 1}/{args.frames}: I = {I:.1f}")

    duration = int(1000 / args.fps)
    images[0].save(
        args.out,
        save_all=True,
        append_images=images[1:],
        duration=duration,
        loop=0,
    )

    shutil.rmtree(tmp)
    print(f"GIF generado: {args.out}  ({args.frames} frames, {args.fps} FPS)")


if __name__ == "__main__":
    main()
