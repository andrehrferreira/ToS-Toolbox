import numpy as np
import matplotlib.pyplot as plt
import json
import sys
from collections import defaultdict


def generate_heightmap_image(json_file_path):
    """
    Generate heightmap image with Unreal 90-degree left rotation correction,
    correct flip, and 4K resolution.
    """
    try:
        print(f"[ToS Python] Loading JSON file: {json_file_path}")
        
        # ✅ Load the JSON
        with open(json_file_path, 'r') as file:
            data = json.load(file)

        # ✅ Access heightmap data
        heightmap_data = data['Heightmap']['Data']
        print(f"[ToS Python] Found {len(heightmap_data)} height data points")

        # ✅ Extract and rotate X, Y, Z points
        points = []
        for entry in heightmap_data:
            x = entry['X']
            y = entry['Y']
            z = np.mean(entry['Z']) if isinstance(entry['Z'], list) else entry['Z']

            # 🔄 Apply Unreal 90 degree left rotation
            x_new = y
            y_new = -x

            points.append((x_new, y_new, z))

        print(f"[ToS Python] Applied Unreal 90-degree left rotation")

        # ✅ Centralize to positive space
        xs = [p[0] for p in points]
        ys = [p[1] for p in points]

        min_x = min(xs)
        min_y = min(ys)

        points = [(x - min_x, y - min_y, z) for (x, y, z) in points]

        # ✅ Flip vertically for top-down view
        max_y = max(p[1] for p in points)
        points = [(x, max_y - y, z) for (x, y, z) in points]

        print("[ToS Python] Applied centering and vertical flip")

        # ✅ Create pivot table manually
        grid_dict = defaultdict(list)
        for x, y, z in points:
            grid_dict[(x, y)].append(z)

        x_coords = sorted(set(x for x, y, z in points))
        y_coords = sorted(set(y for x, y, z in points))

        print(f"[ToS Python] Grid dimensions: {len(x_coords)} x {len(y_coords)}")

        # ✅ Create 2D grid
        grid = np.full((len(y_coords), len(x_coords)), np.nan)

        for i, y in enumerate(y_coords):
            for j, x in enumerate(x_coords):
                if (x, y) in grid_dict:
                    grid[i, j] = np.mean(grid_dict[(x, y)])

        # ✅ Plot Heightmap in 4K resolution
        plt.figure(figsize=(12.8, 7.2), dpi=300)  # 3840x2160 pixels (4K)
        plt.imshow(
            grid,
            origin='lower',  # ✅ FIX FLIP — origin bottom-left
            cmap='terrain',
            extent=[
                min(x_coords), max(x_coords),
                min(y_coords), max(y_coords)
            ]
        )
        plt.colorbar(label='Height (Z)')
        plt.title('Heightmap Export (Unreal Rotation + Flip Correct)', fontsize=18)
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.grid(False)

        # ✅ Save the image
        output_path = json_file_path.replace('.json', '_Heightmap_4K.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"[ToS Python] Heightmap image saved: {output_path}")
        return output_path

    except Exception as e:
        print(f"[ToS Python] ERROR: {str(e)}")
        import traceback
        traceback.print_exc()
        return None


if __name__ == "__main__":
    if len(sys.argv) > 1:
        json_path = sys.argv[1]
        generate_heightmap_image(json_path)
    else:
        print("[ToS Python] ERROR: No JSON file path provided")
