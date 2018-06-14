# Humanoide - Pequi Mecânico

Repositório de softwares desenvolvidos e recursos utilizados pela equipe do Humanoide do [Pequi Mecânico](https://pequimecanico.com/) para participar da [LARC](http://www.cbrobotica.org/) na categoria **RoboCup Humanoid League - KidSize**.

## Dependências

A única toolchain suportada é a versão mais recente do GNU Arm Embedded através do cmake e make. Para instalar as três dependências em distribuições baseadas no Ubuntu execute os comandos:

```
sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
sudo apt-get update
sudo apt-get install gcc-arm-embedded cmake make
```

## Build

Para gerar o arquivo `.bin` que será enviado para o microcontrolador, basta executar os comandos:

```
mkdir build/
cd build/
cmake ..
make
```

Ou executar o script `build.sh` através do comando:

```
./build.sh
```
