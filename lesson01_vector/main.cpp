#include <iostream>
#include <vector>

#include <cstdint>

#include <fstream>
#include <stdexcept>
#include <string>

struct Frame{
	int width;
	int height;
	std::uint64_t sequence;
	std::vector<std::uint8_t> pixels;
};

Frame generate_frame(int width, int height, std::uint64_t sequence){
	Frame frame;
	frame.width = width;
	frame.height = height;
	frame.sequence = sequence;
	frame.pixels.resize(static_cast<std::size_t>(width)*
			static_cast<std::size_t>(height));
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			const std::size_t index = 
				static_cast<std::size_t>(y) * width + x;
			frame.pixels[index] = 
				static_cast<std::uint8_t>((x+sequence)%256);
		}
	}
	return frame;
}	

void save_pgm(const Frame& frame, const std::string& filename){
	std::ofstream output(filename,std::ios::binary);
	if(!output){
		throw std::runtime_error("failed to open output file");
	}
	output << "P5\n" << frame.width << ' ' << frame.height << "\n" << "255\n";
	output.write(reinterpret_cast<const char *>(frame.pixels.data()),
			static_cast<std::streamsize>(frame.pixels.size()));
	if(!output){
		throw std::runtime_error("failed to write frame");
	}

}

int main(){
	std::vector<int> numbers;
	numbers.reserve(20);
	std::cout<<"vector object address:"<<static_cast<const void *>(&numbers)<<'\n';
	std::cout<<"-------------------------------------\n";
	for(int i=0;i<20;++i){
		const int * old_address = numbers.data();
		const std::size_t old_capacity = numbers.capacity();
		numbers.push_back(i);
		const int * new_address = numbers.data();
		std::cout	<< "push" << i
				<< " | size = " << numbers.size()
				<< " | capacity = " << numbers.capacity()
			       	<< " | data = " << static_cast<const void *>(new_address); 	
		if(new_address != old_address){
			std::cout << " | reallocated" << " | old capacity = " << old_capacity;
		}
		std::cout<<'\n';
	}

	Frame frame = generate_frame(320,320,0);
	save_pgm(frame, "frame_000.pgm");
	std::cout<< "sequence: "<< frame.sequence <<'\n';
	std::cout<< "resolution: "<< frame.width << 'x' << frame.height << '\n';
	std::cout<< "bytes: "<< frame.pixels.size() <<'\n';
	
	return 0;
}

