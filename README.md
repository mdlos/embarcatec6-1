<img width=100% src="https://capsule-render.vercel.app/api?type=waving&color=02A6F4&height=120&section=header"/>
<h1 align="center">Embarcatec | Comunicação Serial com RP2040 UART, SPI e I2C</h1>

<div align="center">  
  <img width=40% src="http://img.shields.io/static/v1?label=STATUS&message=EM%20DESENVOLVIMENTO&color=02A6F4&style=for-the-badge"/>
</div>

## Objetivo do Projeto

O principal objetivo deste projeto é utilizar o Raspberry Pi Pico W e explorar as funcionalidades da placa de desenvolvimento BitDogLab.
## 🗒️ Lista de requisitos

- Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.
- LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).
- Botão A conectado à GPIO 5.
- Botão B conectado à GPIO 6.
- Display SSD1306 conectado via I2C (GPIO 14 e GPIO15).

## 🛠 Tecnologias

1. **Git e Github**;
2. **VScode**;
3. **Linguagem C**;
4. **Software de emulação PuTTy.**

## 🔧 Funcionalidades Implementadas:

Entrada de caracteres via PC
• Utilize o Serial Monitor do VS Code para digitar os caracteres.
• Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306.
Observação: Apenas um caractere será enviado de cada vez, não é necessário suportar o envio de strings completas.
• Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve ser exibido, também, na matriz 5x5 WS2812.

Interação com o Botão A
• Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado).
• A operação deve ser registrada de duas formas: o Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
• Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

Interação com o Botão B
• Pressionar o botão A deve alternar o estado do LED RGB Azul (ligado/desligado).
• A operação deve ser registrada de duas formas:
•  Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306
• Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

## 💻 Desenvolvedor
 
<table>
  <tr>
    <td align="center"><img style="" src="https://avatars.githubusercontent.com/u/72825281?v=4" width="100px;" alt=""/><br /><sub><b> Marcio Fonseca </b></sub></a><br />👨‍💻</a></td>
  </tr>
</table>


## 🎥 Demonstração: 
- Para ver o funcionamento do projeto, acesse o vídeo:

Disponivel em: https://youtu.be/3hq430ktYj0
=======
