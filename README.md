# FrequencyDetector

 [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fegecetin%2Ffrequencydetector)](https://hits.seeyoufarm.com)
 ![GitHub](https://img.shields.io/github/license/egecetin/frequencydetector?style=plastic)
 ![GitHub last commit](https://img.shields.io/github/last-commit/egecetin/frequencydetector?style=plastic)
 
 A Modulated/Fixed Frequency Signal Detection algorithm.


## Contents

* [General Info](#general-info)
* [Mathematical Expressions](#mathematical-expressions)
* [Supported Formats](#supported-formats)
* [Interface](#interface)
* [To Do](#to-do)

## General Info

This program aims to detect signals which have constant frequency content such as modulated signals and/or machine driven noises. Since it is based on simple mathematical and vectoral operations, high performance (real-time) results can be obtained. Mathematical background of the algorithm can be found at [Mathematical Expressions Section](#mathematical-expressions).

During the development process FFMpeg 4.3 Windows Build and Intel Performance Libraries used. You can find this FFMpeg package at [Windows builds by Zeranoe](https://ffmpeg.zeranoe.com/builds/). Additionally Qt 5.14.2 and [QCustomPlot](https://www.qcustomplot.com/index.php) used for the user interface. Explanation of the user interface will be given at [Interface Section](#interface) with screenshots.

## Mathematical Expressions

Will be explained later...

## Supported Formats

Since FFMpeg library used for reading audio file lots of video/audio file formats are supported. You can find the list of supported formats with "ffmpeg -formats" command but some examples,

* mp3
* wav
* flac
* ogg

Binary file support will be added in later stages of development.

## Interface

Still under development...

## To Do

I'm planning to develop a STM32 version of this algorithm.
