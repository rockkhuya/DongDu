DongDu
======

A Vietnamese word segmentation tool.

# Giới thiệu
* DongDu là phần mềm tách từ dành cho tiếng Việt.
* Được viết trên ngôn ngữ C++, bởi Lưu Tuấn Anh dưới sự hướng dẫn của Yamamoto Kazuhide.
* DongDu đạt được độ chính xác rất cao (>98%), nhanh và sử dụng ít RAM hơn so với những phần mềm khác.
* DongDu bao gồm cả chương trình tách từ (predictor) và chương trình học máy (learner). Người dùng có thể tự tạo ra dữ liệu cho cá nhân một cách đơn giản.
* Bài viết về thuật toán và phương pháp cụ thể có thể download ở đây (bản tiếng Việt).
* Mọi liên hệ, góp ý hay báo lỗi của chương trình, xin gửi về địa chỉ : rockkhuya@gmail.com

# Build predictor and learner

## Dependency libraries for CentOS 7
For CentOS, install below libraries:
```
yum install -y glibc.i686
yum install -y libstdc++.so.6
```

## Build predictor and learner
Using `make` command to build:
```
make clean compile
```

`predictor` and `learner` are generated in `build` folder.

# Install with vagrant

For easier building. We can make with steps:

  1. Install vagrant from: https://www.vagrantup.com/ (using with virtual box)

  2. Go to root folder of project. Run `vagrant up`. The virtual machine will be created and setup environtment for build. When the virtual machine is run successfully, access it through `vagrant ssh`

  3. After accessed into virtual machine, go to folder `/vagrant`, run the script `./build_learner.sh` or `./build_predictor.sh` to build.
