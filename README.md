# Tales Of Shadowland Toolbox (ToS)

A powerful Unreal Engine 5.5+ plugin designed for exporting NavMesh and heightmap data for server-side anti-cheat validation in the "Tales Of Shadowland" project.

## Features

### 🗺️ NavMesh Export
- **High-resolution surface sampling** with intelligent triangulation
- **Optimized data structure** for server validation
- **Configurable triangle limits** (100-2000 triangles)
- **Integer coordinate precision** for consistent server lookup
- **Minimal memory footprint** for efficient server processing

### 🏔️ Heightmap Export
- **Multi-line trace raycast system** for accurate height detection
- **Configurable sampling resolution** (10-200 Unreal units)
- **Surface type detection** and walkability analysis
- **Consolidated height data** with MinZ/MaxZ values per coordinate
- **Absolute coordinate system** for easy server integration

### ⚙️ Configuration System
- **Persistent project settings** saved in project configuration
- **Customizable export directory** with automatic creation
- **Optional timestamp** in filenames
- **Quality vs performance** trade-offs
- **Easy access** through integrated UI

### 🎨 User Interface
- **Custom toolbar integration** in Unreal Editor
- **Dropdown menu** with organized tools
- **Custom plugin icon** (Icon128.png)
- **Direct access** to plugin settings
- **Intuitive workflow** for level designers

## Installation

1. **Download/Clone** the plugin to your project's `Plugins` folder:
   ```
   YourProject/
   └── Plugins/
       └── ToS/
   ```

2. **Regenerate project files** (right-click .uproject → Generate Visual Studio project files)

3. **Compile** your project in Visual Studio or through Unreal Editor

4. **Enable the plugin** in Edit → Plugins → Project → Tales Of Shadowland Toolbox

## Usage

### Quick Start

1. **Open your level** in Unreal Editor
2. **Click the "ToS" button** in the Assets Toolbar
3. **Choose your export option**:
   - **Export NavMesh**: Exports triangulation + optional heightmap
   - **Export Heightmap**: Exports standalone heightmap data
4. **Configure settings** via "Plugin Settings" option

### Configuration

Access plugin settings through:
- **Toolbar**: ToS → Plugin Settings
- **Menu**: Edit → Project Settings → Plugins → Tales Of Shadowland Toolbox

#### Available Settings

| Setting | Description | Default | Range |
|---------|-------------|---------|-------|
| **Export Directory** | Where files are saved | `ProjectDir/Exported` | Any valid path |
| **Include Timestamp** | Add timestamp to filenames | `false` | true/false |
| **Heightmap Resolution** | Sampling distance in UE units | `50` | 10-200 |
| **Max Triangles** | Triangle limit for NavMesh | `500` | 100-2000 |
| **Include Heightmap with NavMesh** | Bundle heightmap with NavMesh export | `true` | true/false |

### Export Data Structure

#### NavMesh JSON Format (Ultra-Compact)
```json
{
  "LevelName": "MyLevel",
  "N": [{
    "V": [[1000,2000,300],[1050,2000,305],[1000,2050,298]],
    "T": [[0,1,2],[1,2,3]]
  }],
  "H": { /* Optional heightmap, if enabled */ }
}
```

#### Heightmap JSON Format (Optimized)
```json
{
  "LevelName": "MyLevel",
  "Heightmap": {
    "Data": [
      {"X": 1000, "Y": 2000, "Z": [300, 350]},
      {"X": 1050, "Y": 2000, "Z": [305]}
    ],
    "SampleCount": 10000,
    "ValidHits": 8500
  },
  "LevelBounds": {
    "MinX": 0, "MinY": 0, "MinZ": 0,
    "MaxX": 5000, "MaxY": 5000, "MaxZ": 1000
  }
}
```

## Technical Details

### Requirements
- **Unreal Engine**: 5.5 or higher
- **Build Configuration**: Development Editor or DebugGame Editor
- **Platform**: Windows (tested), likely Mac/Linux compatible

### Dependencies
- Core Unreal modules (Engine, Editor, NavigationSystem)
- JSON serialization
- Developer Settings framework
- Tool Menus system

### Performance Considerations
- **Memory Usage**: Heavily optimized for server environments
- **Export Time**: Scales with level size and resolution settings
- **File Size**: Dramatically reduced (80-90% smaller than original)
- **JSON Format**: Ultra-compact formatting without unnecessary whitespace
- **Data Structure**: Minimal overhead - array-based format instead of objects
- **Server Impact**: Minimal - integer coordinates for fast lookup

### File Size Optimizations
- **NavMesh Structure**: `"V":[[X,Y,Z]]` instead of `"Vertices":[{"X":X,"Y":Y,"Z":Z}]`
- **Triangle Format**: `"T":[[0,1,2]]` instead of `"Triangles":[{"V0":0,"V1":1,"V2":2}]`
- **Field Names**: Single characters (`N`, `H`, `V`, `T`) instead of full words
- **Removed Metadata**: No VertexCount, TriangleCount, Bounds objects
- **Heightmap**: Only `{"X":X,"Y":Y,"Z":[Z]}` without MinZ/MaxZ duplication
- **Compact JSON**: No indentation, minimal spaces, condensed format

### Anti-Cheat Integration
The exported data is specifically designed for server-side validation:
- **Position validation**: Check if player coordinates are on valid NavMesh
- **Height validation**: Verify player Z-coordinate against heightmap
- **Movement validation**: Ensure movement follows NavMesh triangles
- **Teleport detection**: Compare against valid walkable surfaces

## Development

### Building from Source
```bash
# Generate project files
UnrealBuildTool.exe -projectfiles -project="YourProject.uproject" -game -rocket -progress

# Build in Visual Studio or
UnrealBuildTool.exe YourProject Win64 Development -project="YourProject.uproject"
```

### Plugin Structure
```
ToS/
├── Resources/
│   └── Icon128.png          # Plugin icon
├── Source/ToS/
│   ├── Public/
│   │   ├── ToS.h           # Module header
│   │   ├── ToSSettings.h    # Configuration class
│   │   ├── ToSStyle.h       # UI styling
│   │   └── NavMeshExporter.h # Core export functionality
│   └── Private/
│       ├── ToSModule.cpp    # Module implementation
│       ├── ToSSettings.cpp  # Settings implementation
│       ├── ToSStyle.cpp     # Style implementation
│       └── NavMeshExporter.cpp # Export logic
├── ToS.uplugin             # Plugin descriptor
└── README.md               # This file
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Troubleshooting

### Common Issues

**Plugin not appearing in toolbar**
- Ensure plugin is enabled in Project Settings
- Restart Unreal Editor
- Check that ToolMenus system is enabled

**Compilation errors**
- Verify Unreal Engine 5.5+ is installed
- Regenerate project files
- Check that all dependencies are available

**Empty export files**
- Ensure NavMesh is built for the level
- Check that the level has NavigationData
- Verify export directory permissions

**Settings not saving**
- Check project file permissions
- Ensure you're in a valid Unreal project
- Restart editor after changing settings

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **Unreal Engine Team** for the excellent navigation and editor frameworks
- **Epic Games** for comprehensive documentation and examples
- **Tales Of Shadowland Team** for project requirements and testing

## Support

For issues, questions, or contributions, please open an issue on the project repository.

---

**Made with ❤️ for the Tales Of Shadowland project** 