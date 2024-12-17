# GAM 550 - Advanced Game Project

## Git Directory Stucture

_`main` branch_ 
- `/` : root directory
    - `source/` 
        - `include/` : holds the **public** header files for your engine or libs visible to prototypes and game(s) 
        - `engine/`
            - `<<game-engine-name>>.vcxproj` : project file for your engine `.lib` (along with the `.vcxproj.filters` file, if any)
            - engine source code goes here
            - optional : some teams choose to split up an engine into distinct parts (.lib files) where each part in a separate directory.  Not recommended, but permissible.
        - `tools/`
            - usually a separate directory for the source code of each tool, with a project file for each tool
        - `tech-prototypes/`
            - `<<tech-prototype-name-1>>/`
                - `<<tech-prototype-name-1>>.vcxproj` : project file for the tech prototype (exe)
                - prototype source code goes here
            - etc. for other tech prototypes
        - `game-prototypes/`
            - `<<game-prototype-name-1>>/`
                - `<<game-prototype-name-1>>.vcxproj` : project file for the game prototype (exe)
                - prototype source code goes here
            - etc. for other game prototypes
        - `game/`
            - `<<game-name-1>>/`
                - `<<game-name-1>>.vcxproj` : project file for the game
                - optional: `<<game-name-1>>.sln` : some teams like to organize each game, test or prototype with its own visual studio solution file, sharing the engine code between them. We will discuss in-class pros and cons of this approach.
                - game C++ source code goes here
            - etc. for other games
        - optional: `<<team-name>>.sln` : some teams like to organize the whole project in a single visual studio solution.  We will discuss in-class pros and cons of this approach.
    - `content/` : content that can be shared by prototypes/game(s)
        - `art/`
            - `models/`
            - `materials/`
            - `textures/`
        - `audio/`
            - `music/`
            - `sounds/` (or `sfx/`)
            - `dialog/` (optional)
        - `code/`    
            - `shaders/`
            - `scripts/`        
        - other directories for engine specific assets shared by prototypes, such as prefabs, level-files, etc.
    - `test/`
        - up to the team how they want to organize the `test` directory
        - when using a monolithic test codebase (this is common in GAM550) there's no directory hierarchy and just one project file
        - some teams choose to have unit tests, one-off tests, regression tests, etc. organizing in subdirectories.  In this case, teams usually have a separate solution file for bootstrapping all the tests.
    - `documentation/`
        - use this directory to store engine documentation, TDDs, GDDs etc.
        - `coding-standards.[md|doc]` : a file containing your coding standards (including links to parts of the ISO coding standard)
    - `external/`
        - each imported library, e.g. fmod or imgui, should be located here under their own directories.  Typically these have their own directory structures.  This directory will be excluded from source-code analysis tools used to measure your individual and group contribution margins.

### notes:
- do not submit `.vcxproj.user` files to the repository; add this extension to your `.gitignore` file
- do not submit any build artifacts (.exe, .lib, etc.)

## Visual Studio Setup
    
- ensure your copy of visual studio is up to date
- platform: x64, available by default, but delete the 32-bit platform(s) with the configuration manager
- configuration: DEBUG & RELEASE, available by default
    - You may also have an instrumented PROFILE configuration for testing performance or for other profiling configurations
    - __no other configurations__
- by default visual studio will select an older C++ standard for any new project files.  Please reset every project to use C++23 by selecting "Preview - Features from the Latest C++ Working Draft"
    - recommend performance mode "permissive-"
- do NOT enable run-time type information
- properly configure your C++ precompiled headers
    - each precompiled header file should contain only `#include` directives for `std` library headers and `external` projects.
- make only minimal, necessary changes to source code under `external`
- editor:
    - please prefer spaces over tabs.  Tabs are ugly in GitHub.  You can configure your IDE to use spaces by default.
    - indent consistently: 2, 3, or 4 spaces
    - you may use UTF-8 encoding for your source code

