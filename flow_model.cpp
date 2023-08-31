#include <iostream>
#include <vector>

#include <torch/script.h>

int main() {

  // Load pretrained flow model
  auto flow = torch::jit::load("/home/patrick/CMS/FlashSim/Code/nflows_cpp_v5/flow_model.pt");
  flow.eval();

  int num_iter = 5000;

  for (int i = 0; i < num_iter; ++i) {

    // Generate toy dataset
    auto x = torch::rand({128, 2}) * 3.0 - 1.5; 
    auto y = torch::zeros({128, 1});

    if ((i + 1) % 500 == 0) {
      
      // Sample from prior
      auto zgrid0 = flow({torch::zeros({8192, 1})}).toTensor().exp();
      auto zgrid1 = flow({torch::ones({8192, 1})}).toTensor().exp();
      
      // Reshape samples into a grid
      zgrid0 = zgrid0.reshape({128, 128});
      zgrid1 = zgrid1.reshape({128, 128});
      
      // Plot samples
      // std::cout << "Iteration " << i+1 << "\n";
      // std::cout << zgrid0 << "\n";
      // std::cout << zgrid1 << "\n";
 

      // Save samples to CSV files
      std::ofstream file0("dataCpp_after_Cpp_reading_zgrid0.csv");
      std::ofstream file1("dataCpp_after_Cpp_reading_zgrid1.csv");
      
      file0 << "x0,x1\n";
      for (int i = 0; i < zgrid0.size(0); ++i) {
        file0 << zgrid0[i][0].cpu().item<float>() << "," << zgrid0[i][1].cpu().item<float>() << "\n";
      }
      file0.close();
      
      file1 << "x0,x1\n";
      for (int i = 0; i < zgrid1.size(0); ++i) {
        file1 << zgrid1[i][0].cpu().item<float>() << "," << zgrid1[i][1].cpu().item<float>() << "\n";
      }
      file1.close();

    }

  }

  return 0;
}
