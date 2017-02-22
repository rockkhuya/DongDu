Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/trusty64"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = "1024"
  end

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    sudo locale-gen UTF-8
    apt-get install -y g++
    apt-get install -y libblas-dev liblapack-dev
    apt-get install -y gfortran
  SHELL
end
