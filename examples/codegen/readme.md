## Project Overview

This project aims to generate code from RTTR data in various source files. Here’s why this is useful:

1) Immediate Availability: Tagging something in the source makes it instantly available for scripting, or serialization to disk or over the network.
2) Metadata Control: Use metadata to control serialization and visibility in the scripting environment.

Previously, achieving this required extensive boilerplate code, often copied and pasted, leading to many issues. With this new system, you simply mark which types are exposed and use them directly in your project.
Implementation Steps

### Step 1: Build the Library and Codegen Project

1) Object Library: This is where all your types are declared. It doesn’t need to declare a main function.
2) Codegen Project: This project declares a main function and links to the object library, enabling the codegen tool to recognize all types registered with SimpleRTTR through automatic static registration.

While it's possible to export type data to JSON and parse it into a codegen tool, we chose this route for simplicity.

### Step 2: Manage the Build System

A challenge arises because we can't predict which source files will be generated ahead of compile time. That's partly our fault because we choose to output source files based on type name, not using the types source file as a way to encode the final output file. Here’s how we handle it:

1) Code Generation:
    * The CodeGen tool processes each type and generates new source files based on specified templates.
    * It uses the Type::Hash() function to detect changes in types and caches results to track additions, removals, or modifications.

2) CMakeLists Updates:
    * If types have been added or removed, it updates the CMakeLists.txt file, forcing a reconfigure.
    * If there are no new or removed types, and the CMakeLists template is unmodified, it skips reconfiguration and proceeds to compilation.

3) Efficient Compilation:
    * Source files are updated only if they have been added or modified, ensuring minimal code compilation.

### Step 3: Link Everything Together

Once compiled, all the code is linked into a single object library. We declare a template function in a header file that has no implementation, then within the source templates, we use template specialization to define the function for the type. This allows us to call the function from the base project, which links to the newly created code generated library.

This library is then linked to the base project, allowing the bindings to be called from within the target application.  

By following these steps, we streamline the process of exposing types for scripting and serialization, reducing the need for repetitive boilerplate code.

## Conclusion

This project demonstrates that RTTR can be used to generate code from data in source files. By declareing a type like this:

```cpp

```