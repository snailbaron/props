#include <props.hpp>

#include <iostream>
#include <string>

class Window {
public:
    Window()
    {
    }

    void setName(std::string name)
    {
        this->name = std::move(name);
    }

    void setSize(int width, int height)
    {
    }

    props::Property<std::string> name;
    props::Property<int> x = 0;
};

int main()
{
    auto window = Window{};
    window.name = "abc";
    std::cout << "window name: " << window.name << "\n";
    window.name += "def";
    std::cout << "window name: " << window.name << "\n";

    window.x += 10;
    std::cout << window.x + 3 << "\n";
    std::cout << (window.x == 10) << "\n";
}
