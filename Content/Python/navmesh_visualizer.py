import json
import sys
import os
import subprocess


def install_package(package):
    """Install a package if missing"""
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", package])
        return True
    except:
        return False


def generate_navmesh_visualization(json_file_path):
    """
    Generate NavMesh visualization with Unreal 90 degree left rotation correction
    """
    try:
        print(f"[ToS Python] Loading NavMesh JSON file: {json_file_path}")

        # Import required packages
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

        # Load NavMesh JSON
        with open(json_file_path, 'r') as f:
            data = json.load(f)

        print(f"[ToS Python] JSON loaded successfully")

        # Extract vertices
        navmesh_data = data.get('N', [])
        vertices = []

        for poly in navmesh_data:
            for vert in poly.get('V', []):
                vertices.append(vert)

        vertices = np.array(vertices, dtype=np.float32)
        print(f"[ToS Python] Extracted {len(vertices)} vertices")

        if len(vertices) == 0:
            print("[ToS Python] ERROR: No vertices found in NavMesh data")
            return None

        # 🔥 Apply Unreal rotation correction (90 degrees left)
        x_new = vertices[:, 1]
        y_new = -vertices[:, 0]

        vertices[:, 0] = x_new
        vertices[:, 1] = y_new

        print("[ToS Python] Applied 90-degree left rotation (Unreal correction)")

        # 🔄 Centralize to positive space
        vertices[:, 0] -= vertices[:, 0].min()
        vertices[:, 1] -= vertices[:, 1].min()

        # 🔁 Optional: Flip vertically for top-down view
        vertices[:, 1] = vertices[:, 1].max() - vertices[:, 1]

        print("[ToS Python] Applied centering and vertical flip")

        # Plot NavMesh
        plt.figure(figsize=(12.8, 7.2), dpi=300)
        plt.scatter(vertices[:, 0], vertices[:, 1], c=vertices[:, 2],
                    cmap='viridis', s=10)

        plt.title('NavMesh Visualization (Unreal Rotation Corrected)')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.colorbar(label='Z Height')
        plt.grid(True)
        plt.axis('equal')

        # Save image
        output_path = json_file_path.replace('.json', '_NavMesh.png')
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"[ToS Python] NavMesh visualization saved: {output_path}")
        return output_path

    except Exception as e:
        print(f"[ToS Python] ERROR: {str(e)}")
        import traceback
        traceback.print_exc()
        return None


if __name__ == "__main__":
    if len(sys.argv) > 1:
        json_path = sys.argv[1]
        generate_navmesh_visualization(json_path)
    else:
        print("[ToS Python] ERROR: No JSON file path provided")
