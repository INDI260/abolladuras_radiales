#!/usr/bin/env python3
import argparse
from datetime import datetime
import numpy as np


def perlin_noise(x, y, scale, octaves, seed=42):
    rng = np.random.default_rng(seed)
    z = np.zeros_like(x)
    amplitude = 1.0
    frequency = 1.0 / scale
    max_amplitude = 0.0

    for _ in range(int(octaves)):
        gx = rng.uniform(-1, 1, 512)
        gy = rng.uniform(-1, 1, 512)

        sx = x * frequency
        sy = y * frequency
        ix = np.floor(sx).astype(int)
        iy = np.floor(sy).astype(int)
        fx = sx - ix
        fy = sy - iy
        u = fx * fx * (3 - 2 * fx)
        v = fy * fy * (3 - 2 * fy)

        n00 = gx[(ix) % 512] * (sx - ix) + gy[(iy) % 512] * (sy - iy)
        n10 = gx[(ix + 1) % 512] * (sx - (ix + 1)) + gy[(iy) % 512] * (sy - iy)
        n01 = gx[(ix) % 512] * (sx - ix) + gy[(iy + 1) % 512] * (sy - (iy + 1))
        n11 = gx[(ix + 1) % 512] * (sx - (ix + 1)) + gy[(iy + 1) % 512] * (sy - (iy + 1))

        layer = n00 + u * (n10 - n00) + v * (n01 - n00 + u * (n00 - n10 - n01 + n11))
        z += amplitude * layer
        max_amplitude += amplitude
        amplitude *= 0.5
        frequency *= 2

    return z / max_amplitude


def write_obj(filename, points):
    with open(filename, "w") as f:
        for p in points:
            f.write(f"v {p[0]:.6f} {p[1]:.6f} {p[2]:.6f}\n")


def write_dents(filename, dents):
    with open(filename, "w") as f:
        for cx, cy, r, intensity in dents:
            f.write(f"{cx:.6f},{cy:.6f};{r:.6f};{intensity:.6f}\n")


def main():
    p = argparse.ArgumentParser(
        description="Genera un par (.obj + dents.txt) para abolladuras radiales")

    p.add_argument("filas", type=int)
    p.add_argument("columnas", type=int)
    p.add_argument("abolladuras", type=int)

    p.add_argument("--x-range", nargs=2, type=float, default=[0, 200])
    p.add_argument("--y-range", nargs=2, type=float, default=[0, 200])
    p.add_argument("--r-range", nargs=2, type=float, default=[10, 50])
    p.add_argument("--i-range", nargs=2, type=float, default=[0, 20])

    mode = p.add_mutually_exclusive_group()
    mode.add_argument("--flat", action="store_true")
    mode.add_argument("--random", nargs=2, type=float, metavar=("ZMIN", "ZMAX"),
                      default=[-5, 5])
    mode.add_argument("--perlin", nargs=2, type=float,
                      metavar=("SCALE", "OCTAVES"))

    p.add_argument("--out", help="Output filename prefix (default: timestamp)")

    args = p.parse_args()

    out_prefix = args.out or datetime.now().strftime("%Y%m%d_%H%M%S")
    obj_file = f"{out_prefix}.obj"
    dents_file = f"{out_prefix}_dents.txt"

    xmin, xmax = args.x_range
    ymin, ymax = args.y_range

    rows, cols = args.filas, args.columnas
    xs = np.linspace(xmin, xmax, cols)
    ys = np.linspace(ymin, ymax, rows)
    xx, yy = np.meshgrid(xs, ys)
    xx = xx.ravel()
    yy = yy.ravel()

    if args.flat:
        zz = np.zeros_like(xx)
    elif args.perlin is not None:
        scale, octaves = args.perlin
        zz = perlin_noise(xx, yy, scale, int(octaves), seed=42)
        zz = zz * 20
    else:
        zmin, zmax = args.random
        rng = np.random.default_rng(0)
        zz = rng.uniform(zmin, zmax, len(xx))

    points = np.column_stack([xx, yy, zz])
    write_obj(obj_file, points)

    rmin, rmax = args.r_range
    imin, imax = args.i_range
    rng = np.random.default_rng(1)
    dents = []
    for _ in range(args.abolladuras):
        cx = rng.uniform(xmin, xmax)
        cy = rng.uniform(ymin, ymax)
        r = rng.uniform(rmin, rmax)
        intensity = rng.uniform(imin, imax)
        dents.append((cx, cy, r, intensity))
    write_dents(dents_file, dents)

    print(f"Generados: {obj_file} (grid {rows}x{cols}) y {dents_file}")


if __name__ == "__main__":
    main()
