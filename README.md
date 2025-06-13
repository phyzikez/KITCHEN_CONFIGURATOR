# KITCHEN_CONFIGURATOR
This is a demo of an engine-like application for configuring kitchen furniture. It has two main systems: inner core logic and UI, both written completely in C++. Vertex mesh coordinates are imported from 3ds Max, and 2D graphic elements are created in Photoshop. The app presents an empty kitchen scene in 3D space where the user can add and configure different modules, either separately or in combination with others. Rendering of the scene is implemented via OpenGL.

Each module consists of a set of meshes, with each mesh loaded from an .obj file as an array of vertices, textures, and normal coordinates. Users can interact with modules and their individual parts in various ways through the UI system, which consists of flexible panels that represent common scene features, separate module functionalities, and individual part interactions. A few modules and parts have similar features, while others have unique options based on their real-life functionality. 

Each module is a separate class inherited from the base Module class, which contains common features and options. Modules with facades are implemented within the Facade class with its own subclasses. Shared functionality and features are also structured using additional interfaces and enumerations.

Here is a list of options and features implemented in the application:

Core Logic:
<br>•	Dynamic loading of 3D meshes
<br>•	Vertex, texture, and material class-based system
<br>•	Manual manipulation of mesh vertices, normals, and textures (moving, scaling, alpha blending, etc.)
<br>•	Random object spawning
<br>•	Exporting configured scenes
<br>•	Different render modes (diffuse, wireframe, ambient occlusion)
<br>•	Flexible shader system (switching shaders, light settings)
<br>•	Animations (meshes, camera, UI elements)
<br>•	Flexible camera system (free mode, targeting, interpolating)
<br>•	User tip event-based system
<br>•	Sound and text event-based system
<br>•	User interaction via mouse/keyboard events in a 2D UI space
<br>•	Coordinated interaction between UI, module logic, tip system, and sound effects

Module Options:
<br>•	Smart resizing of entire modules or individual parts (in available directions based on interaction with other dependent modules/parts)
<br>•	Smart movement of entire modules or individual parts (in available directions based on interaction with other dependent modules/parts)
<br>•	Visibility toggling
<br>•	Flexible facade configuration system (including different types of rotations, such as combined tandem rotation)
<br>•	Material switching for module parts
<br>•	Prevention of excessive scaling, moving, opening/closing, and blocking of modules/parts
<br>•	Flexible dynamic mesh-switching system (e.g., in cargo modules where both static and dynamic parts are combined as options)
<br>•	Switching between different types of meshes (facade door mesh, drawer sides, etc.)
<br>•	Random mesh pattern generation algorithm

UI Logic:
<br>•	A set of different panels representing four types of user mouse/keyboard interaction events (hovering, key-up, key-down, element-over), each accompanied by a unique sound effect (with appropriate duration and prevention of multiple plays when events occur too quickly)
<br>•	Flexible upper-left circular panel containing general scene options (light intensity, camera targeting, render mode, sound volume, kitchen export)
<br>•	Bottom main panel displaying user tips, a button to hide/show a flexible panel for adding or removing modules, and checkboxes for toggling the visibility of other main panels to enhance the user’s scene-view experience
<br>•	Bottom-centered panel for switching between different modules and their parts (previous/next), entering/exiting a module with smooth interpolated camera targeting animation
<br>•	Several switchable feature panels displaying the user’s selected option via looped circular animations
<br>•	Material panel displaying available materials for each module part
<br>•	Mesh panel for switching between different facade geometry types in modules with enabled facades
<br>•	Rotation panel for configuring facade rotations in modules with enabled facades
<br>•	Handle panel for adding/removing handles in modules with enabled facades
<br>•	Two visibility-setting buttons for unique module features: switching cargo content types and drawer side mesh styles
<br>•	Flexible scale panel for scaling module parts along each axis in both directions
<br>•	Divider panel containing several pre-set drawer space-split grids and an option for random split-pattern generation
<br>•	Opening module facades, drawers, and cargo elements via array keyboard button events (left mouse button and right mouse button) for intuitive interaction
<br>•	“Quick” manual camera switching between targeted and free-view modes using the middle mouse button
<br>•	Two timed and separately functioning text panels to improve user experience: informational and warning. The informational panel appears when the user moves or scales module parts, fully opens or closes a facade door or inner drawers, etc. The warning panel appears when the user attempts an invalid action (such as opening a blocked drawer, opening/closing a module door blocked by another module part, exceeding mesh scaling limits, etc.)
<br>•	Splash screen represented by a progress bar that fills dynamically according to core mesh information loading progress

The demonstration of working with app-scene is shown in attached video.
<br>https://drive.google.com/file/d/1Yv1thyl1_uyKOU5w-YW1lrpGviW7l7rI/view?usp=drive_link
