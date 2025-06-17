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

def generate_navmesh_visualization(json_file_path):
    """
    Generate NavMesh visualization using the exact code provided by user
    """
    try:
        print(f"[ToS Python] Loading NavMesh JSON file: {json_file_path}")
        
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
        
        # Carregar dados do NavMesh
        with open(json_file_path, 'r') as f:
            data = json.load(f)

        print(f"[ToS Python] JSON loaded successfully")

        # Extrair vértices
        navmesh_data = data.get('N', [])
        vertices = []

        for poly in navmesh_data:
            for vert in poly.get('V', []):
                vertices.append(vert)

        vertices = np.array(vertices)
        print(f"[ToS Python] Extracted {len(vertices)} vertices")

        if len(vertices) == 0:
            print("[ToS Python] ERROR: No vertices found in NavMesh data")
            return None

        # Centralizar nos positivos
        vertices[:, 0] += abs(vertices[:, 0].min())
        vertices[:, 1] += abs(vertices[:, 1].min())

        # Flip vertical
        vertices[:, 1] = vertices[:, 1].max() - vertices[:, 1]

        print(f"[ToS Python] Applied transformations: centering, vertical flip")

        # Plotar NavMesh
        plt.figure(figsize=(12, 10))
        plt.scatter(vertices[:, 0], vertices[:, 1], c=vertices[:, 2], cmap='viridis', s=10)
        plt.title('NavMesh Visualization - Vertical Flip (Correct)')
        plt.xlabel('X (Final)')
        plt.ylabel('Y (Final)')
        plt.colorbar(label='Z Height')
        plt.grid(True)
        plt.axis('equal')

        # Salvar imagem
        output_path = json_file_path.replace('.json', '_NavMesh.png')
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
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