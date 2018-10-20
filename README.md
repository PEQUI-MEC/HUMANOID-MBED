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

## Protocolo de Comunicação

A comunicação com o alto nível acontece através de serial (UART) utilizando um protocolo próprio composta de alguns comandos. Todas as mensagens são precedidas por um cabeçalho que indica o protocolo, o comando e algumas informações de verificação de erro.

### Cabeçalho

| Byte(s) | Nome | Padrão | Descrição |
|:---:|:---:|:---:|:---|
| 0 | Protocolo | `0xE8` | Indica qual protocolo está sendo transmitido |
| 1 | Comando (ID) | | Indica qual o comando que está sendo enviado na mensagem |
| 2 | Tamanho | | Tamanho da mensagem que está sendo transmitida, incluindo o cabeçalho |
| 3 | Checksum | | Byte calculado utilizando operações XOR da seguinte forma: `checksum = comando ^ tamanho ^ dado[0] ^ ... ^ dado[N-1]` |
| 4 - N | Dados | | Dados referentes ao comando da mensagem |

### Comandos

| Comando | ID | N° Bytes | Significado |
|:---:|:---:|:---:|:---|
| ERROR | `0x01` | 0 | Indica algum erro na transmissão ou reconhecimento da ultima mensagem |
| UPDATE | `0x02` | 41 | Transmite a posição de cada um dos motores e do nível de tensão da bateria |
| GOAL | `0x03` | 40 | Transmite as posições desejadas para cada motor |

### Comando UPDATE

| Byte(s) | Descrição |
|:---:|:---|
| 0 - 3| Cabeçalho |
| 4 - 39 | Posições dos 18 motores do corpo |
| 40 - 43 | Posições dos motores do gimbal, sendo a primeira posição o *pitch* e a segunda o *yaw* |
| 44 | Nível de tensão da bateria |

As posições dos motores do corpo e do gimbal são transmitidas em conjuntos de 2 bytes em que o byte nas posições `N` e `N+1` indicam a posição do motor com ID `((N - 4)/2) + 1`. O byte `N` é a parte superior da posição (MSB) e o byte `N+1` é a parte inferior da posição (LSB).

### Comando GOAL

| Byte(s) | Descrição |
|:---:|:---|
| 0 - 3| Cabeçalho |
| 4 - 39 | Posições dos 18 motores do corpo |
| 40 - 43 | Posições dos motores do gimbal, sendo a primeira posição o *pitch* e a segunda o *yaw* |

As posições dos motores do corpo são transmitidas seguindo a mesma regra do comando UPDATE.
