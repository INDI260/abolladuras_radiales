#!/usr/bin/env python3
import sys
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
import numpy as np


def read_obj(filename):
    pts = []
    with open(filename) as f:
        for line in f:
            if line.startswith("v "):
                _, x, y, z = line.split()
                pts.append((float(x), float(y), float(z)))
    return np.array(pts)


def read_dents(filename):
    dents = []
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            center, radius, intensity = line.split(";")
            cx, cy = center.split(",")
            dents.append((float(cx), float(cy), float(radius), float(intensity)))
    return dents


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} puntos.obj dents.txt")
        sys.exit(1)

    pts = read_obj(sys.argv[1])
    dents = read_dents(sys.argv[2])

    fig, ax = plt.subplots(figsize=(10, 8))

    sc = ax.scatter(pts[:, 0], pts[:, 1], c=pts[:, 2], cmap="viridis",
                    s=20, edgecolors="none", zorder=2)
    cbar = plt.colorbar(sc, ax=ax, label="Altura z")

    vmin = min(pts[:, 2])
    vmax = max(pts[:, 2])

    for cx, cy, r, intensity in dents:
        color = plt.cm.Reds(1.0 - intensity * 0.7)
        circle = Circle((cx, cy), r, facecolor=color, edgecolor="black",
                        linestyle="--", linewidth=1.2, alpha=0.45, zorder=3)
        ax.add_patch(circle)
        ax.plot(cx, cy, "k+", markersize=8, zorder=4)
        ax.annotate(f"I={intensity:.2f}", (cx, cy),
                    xytext=(5, 5), textcoords="offset points",
                    fontsize=8, color="black", zorder=5)

    ax.set_aspect("equal")
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_title(f"Puntos: {len(pts)}, Abolladuras: {len(dents)}")

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    out = f"vis_{timestamp}.png"
    fig.savefig(out, dpi=150, bbox_inches="tight")
    print(f"Saved: {out}")


if __name__ == "__main__":
    main()
