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

Para gerar o arquivo `.bin` que será enviado para o microcontrolador, basta executar o script `build.sh` através do comando:

```
./build.sh
```

Ou executar a sequência de comandos comandos:

```
mkdir build/
cd build/
cmake ..
make
```

## Protocolo de Comunicação

A comunicação com o alto nível acontece através de serial (UART) utilizando um protocolo em que as mensagens são diferentes dependendo da direção em que são enviadas. Todas as mensagens são compostas por um cabeçalho (*header*), que inclui um checksum para verificação de erro, os dados e um rodapé (*footer*). Os dados com multiplos bytes são enviados no formato *big-endian*.

### Micro p/ Controlador

| Byte(s) | Informação | Padrão | Descrição |
|:---:|:---:|:---:|:---|
| 0 | Header | `0xF1` | Byte que indica o inicio da mensagem |
| 1 | Header | `0xF1` | Byte que indica o inicio da mensagem |
| 2 | Checksum | | Byte calculado utilizando operações XOR bit a bit dos dados: `checksum = dado[0] ^ ... ^ dado[N-1]` |
| 3 - 4 | Ângulo | | Pitch do Gimbal |
| 5 - 6 | Ângulo | | Yaw do Gimbal |
| 7 - 8 | Ângulo | | Ângulo do motor do Gimbal que atua no pitch |
| 9 - 10 | Ângulo | | Ângulo do motor do Gimbal que atua no yaw |
| 11 | Tensão | | Tensão de referẽncia da bateria |
| 12 | Footer | `0xF2` | Byte que indica o fim da mensagem |
| 13 | Footer | `0xF2` | Byte que indica o fim da mensagem |

### Controlador p/ Micro

| Byte(s) | Informação | Padrão | Descrição |
|:---:|:---:|:---:|:---|
| 0 | Header | `0xF1` | Byte que indica o inicio da mensagem |
| 1 | Header | `0xF1` | Byte que indica o inicio da mensagem |
| 2 | Checksum | | Byte calculado utilizando operações XOR bit a bit dos dados: `checksum = dado[0] ^ ... ^ dado[N-1]` |
| 3 - 38 | | Posições dos 18 motores do corpo |
| 39 - 40 | | Posição do motor do Gimbal que atua no pitch |
| 41 - 42 | | Posição do motor do Gimbal que atua no yaw |
| 43 | Footer | `0xF2` | Byte que indica o fim da mensagem |
| 44 | Footer | `0xF2` | Byte que indica o fim da mensagem |
