

#include "config/config.h"
#include "gui/gui.h"
#include "imgui.h"

#include <atomic>

void test();

std::atomic_bool done = false;
// Main code
int main(int argc, char **argv) {
    // if (argc > 1) {
    //     config_init(argv[1]);
    // } else {
    //     config_init(nullptr);
    // }
    gui_init(100, 100, 1280, 800, false);
    while (!done.load(std::memory_order_acquire)) {
        gui_frame(test);
    }
    gui_clean();
    // config_save();
    return 0;
}

// Demo code
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
void test() {
    ImGuiIO &io = ImGui::GetIO();
    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
        // and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can
        // use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);
        // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3(
            "clear color",
            (float *)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most
            // widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
        ImGui::Begin(
            "Another Window",
            &show_another_window); // Pass a pointer to our bool variable (the
        // window will have a closing button that will
        // clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}