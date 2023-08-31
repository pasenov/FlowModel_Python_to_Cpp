# FlowModel_Python_to_Cpp

Instructions:

1. Create a working directory called FlowModel_Python_to_Cpp.
2. In FlowModel_Python_to_Cpp, create a directory called nflows_mod. Download conditional_moons_mod.py there. Run inside that directory:
python3 conditional_moons_mod.py
3. Make a new directory inside FlowModel_Python_to_Cpp called nflows_read_pt_model
4. Enter it and copy flow_model.pt from nflows_mod there. Download read_pt.py there.
5. Run inside that directory:
python3 read_pt.py
6. Make a new directory inside FlowModel_Python_to_Cpp called nflows_cpp
7. Enter it and copy flow_model.pt from nflows_mod there. Download flow_model.cpp and CMakeLists.txt there.
8. Make a directory called build in there. Enter build and run inside:
cmake -DCMAKE_PREFIX_PATH=/home/patrick/CMS/libtorch/libtorch ..
changing /home/patrick/CMS/libtorch/libtorch to the location on your computer where libtorch is located.
9. Then run:
make -j16
adjusting the above command to match the availability of CPU cores.
10. Then run:
./flow_model
11. Copy all 6 .csv files from the above subdirectories to a new directory inside FlowModel_Python_to_Cpp called plot. Download plot_nflows_ROOT.C there.
12. Run inside that directory:
for f in *.csv; do sed -i '1d' "$f"; done
BUT ONLY THE VERY FIRST TIME BEFORE RUNNING THE ROOT PROGRAM!
13. Run inside that directory:
root -l plot_nflows_ROOT.C
14. You should now see a file similar to Moons.pdf where the two half-moons are plotted next to each other: a) after the initial training; b) after loading the trained .pt model and reusing it in Python; c) after loading the trained .pt model and reusing it in C++. If for any reason you don't see it, please contact me!
