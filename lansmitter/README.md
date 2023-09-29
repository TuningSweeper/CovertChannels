# Lansmitter - Covert Channel Communication

Lansmitter is a Windows tool designed for covert channel communication by leveraging changes in Ethernet interface speed. This enables to transmit information in a hidden manner by manipulating network interface characteristics, which are reflected on the RF spectrum. These are easy to receive using affordable Software-Defined Radios (SDRs).

## Compatibility

Worked once on my machine.

## Inspiration

Lansmitter is inspired by the LANTENNA research project, which explores similar concepts of covert communication using network-related properties. See https://arxiv.org/abs/2110.00104

## Demo Video

[![Simple demo]](lansmitter-demo1.mp4). This is captured with RTL-SDR.

## Usage

Run Lansmitter in administrator PowerShell:
```
./lansmitter.ps1 "message"
```
Also, there are dit.ps1 and dash.ps1 which send dit and dash.

These artefacs may show up on frequencies such as 10 MHz, 125 MHz, 250 Mhz, 1 GHz or something like that. Maybe. Or maybe not