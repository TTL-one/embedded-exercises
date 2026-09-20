#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

class Frame
{
public:
    Frame(int width, int height, std::uint64_t sequence)
        : width_(width),
          height_(height),
          sequence_(sequence),
          pixels_(static_cast<std::size_t>(width) *
                      static_cast<std::size_t>(height),
                  0)
    {
        print_state("constructor");
    }

    Frame(const Frame& other)
        : width_(other.width_),
          height_(other.height_),
          sequence_(other.sequence_),
          pixels_(other.pixels_)
    {
        print_state("copy constructor");
    }

    Frame(Frame&& other) noexcept
        : width_(other.width_),
          height_(other.height_),
          sequence_(other.sequence_),
          pixels_(std::move(other.pixels_))
    {
        other.width_ = 0;
        other.height_ = 0;
        other.sequence_ = 0;
        print_state("move constructor");
    }

    ~Frame()
    {
        print_state("destructor");
    }

    std::size_t size() const
    {
        return pixels_.size();
    }

    const void* data_address() const
    {
        return pixels_.data();
    }

private:
    void print_state(const char* operation) const
    {
        std::cout << operation
                  << " | sequence = " << sequence_
                  << " | data = " << static_cast<const void*>(pixels_.data())
                  << " | size = " << pixels_.size()
                  << '\n';
    }

    int width_;
    int height_;
    std::uint64_t sequence_;
    std::vector<std::uint8_t> pixels_;
};

int main()
{
    std::cout << "===== construct a =====\n";
    Frame a(320, 240, 1);

    std::cout << "\n===== copy a to b =====\n";
    Frame b = a;

    std::cout << "\n===== move a to c =====\n";
    Frame c = std::move(a);

    std::cout << "\n===== inspect objects =====\n";
    std::cout << "a: data = " << a.data_address()
              << ", size = " << a.size() << '\n';
    std::cout << "b: data = " << b.data_address()
              << ", size = " << b.size() << '\n';
    std::cout << "c: data = " << c.data_address()
              << ", size = " << c.size() << '\n';

    std::cout << "\n===== leave main =====\n";
    return 0;
}

/*
实验二：完成实验一后，用下面的main替换上面的main。

int main()
{
    std::vector<Frame> frames;
    frames.reserve(3);

    std::cout << "===== push lvalue =====\n";
    Frame first(320, 240, 1);
    frames.push_back(first);

    std::cout << "\n===== push moved lvalue =====\n";
    Frame second(320, 240, 2);
    frames.push_back(std::move(second));

    std::cout << "\n===== emplace =====\n";
    frames.emplace_back(320, 240, 3);

    std::cout << "\n===== leave main =====\n";
    return 0;
}
*/

