import json
import sys
import os
import subprocess

def install_package(package):
    """Try to install a package using pip"""
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", package])
        return True
    except:
        return False

def generate_heightmap_image(json_file_path):
    """
    Generate heightmap image - try to import required packages, install if needed
    """
    try:
        print(f"[ToS Python] Loading JSON file: {json_file_path}")
        
        # Try to import required packages
        try:
            import numpy as np
            print("[ToS Python] numpy imported successfully")
        except ImportError:
            print("[ToS Python] numpy not found, trying to install...")
            if install_package("numpy"):
                import numpy as np
                print("[ToS Python] numpy installed and imported")
            else:
                print("[ToS Python] ERROR: Failed to install numpy")
                return None
        
        try:
            import matplotlib.pyplot as plt
            print("[ToS Python] matplotlib imported successfully")
        except ImportError:
            print("[ToS Python] matplotlib not found, trying to install...")
            if install_package("matplotlib"):
                import matplotlib.pyplot as plt
                print("[ToS Python] matplotlib installed and imported")
            else:
                print("[ToS Python] ERROR: Failed to install matplotlib")
                return None
        
        # Load the JSON
        with open(json_file_path, 'r') as file:
            data = json.load(file)

        # Access heightmap data
        heightmap_data = data['Heightmap']['Data']
        print(f"[ToS Python] Found {len(heightmap_data)} height data points")

        # Extract X, Y, Z points
        points = []
        for entry in heightmap_data:
            x = entry['X']
            y = entry['Y']
            # Handle both list and single Z values
            if isinstance(entry['Z'], list):
                z = sum(entry['Z']) / len(entry['Z'])  # Simple average without numpy
            else:
                z = entry['Z']
            points.append((x, y, z))

        print(f"[ToS Python] Created points list with {len(points)} points")

        # Create grid manually (replacing pandas pivot_table)
        # Group by X,Y coordinates and average Z values
        grid_dict = {}
        for x, y, z in points:
            key = (x, y)
            if key not in grid_dict:
                grid_dict[key] = []
            grid_dict[key].append(z)
        
        # Get unique X and Y coordinates and sort them
        x_coords = sorted(set(x for x, y, z in points))
        y_coords = sorted(set(y for x, y, z in points))
        
        print(f"[ToS Python] Grid dimensions: {len(x_coords)} x {len(y_coords)}")
        
        # Create 2D grid
        grid = []
        for y in y_coords:
            row = []
            for x in x_coords:
                if (x, y) in grid_dict:
                    # Average the Z values for this coordinate
                    z_values = grid_dict[(x, y)]
                    avg_z = sum(z_values) / len(z_values)
                    row.append(avg_z)
                else:
                    row.append(float('nan'))
            grid.append(row)
        
        # Convert to numpy array
        grid = np.array(grid)
        
        # Plot Heightmap (exactly like ChatGPT code)
        plt.figure(figsize=(12, 10))
        plt.imshow(
            grid,
            origin='upper',  # Correct orientation for Unreal
            cmap='terrain',
            extent=[
                min(x_coords), max(x_coords),
                min(y_coords), max(y_coords)
            ]
        )
        plt.colorbar(label='Height (Z)')
        plt.title('Heightmap Export (Correct Orientation)')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.grid(False)
        
        # Save the image
        output_path = json_file_path.replace('.json', '_Python.png')
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        plt.close()  # Close to free memory
        
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