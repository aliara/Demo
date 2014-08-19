/*
 * constants.h
 *
 *  Created on: 15/08/2014
 *      Author: programador2
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Debugging
const bool kPlotVectorField = true;

// Size constants
const int kEyePercentTop = 25;
const int kEyePercentSide = 13;
const int kEyePercentHeight = 30;
const int kEyePercentWidth = 35;

// Preprocessing
const bool kSmoothFaceImage = false;
const float kSmoothFaceFactor = 0.005;

// Algorithm Parameters
const int kFastEyeWidth = 50;
const int kWeightBlurSize = 5;
const bool kEnableWeight = false;
const float kWeightDivisor = 150.0;
const double kGradientThreshold = 50.0;

// Postprocessing
const bool kEnablePostProcess = true;
const float kPostProcessThreshold = 0.97;

// Eye Corner
const bool kEnableEyeCorner = false;

//Config
const bool GBStatus = true;

const char hola = 0;

//Debug
const bool debugL0=true;
const bool debugL1=false;
const bool debugL2=false;
const bool debugL3=false;
const bool debugL4=false;
const bool debugL5=false;

//Umbrales
const int persistCuadros = 1;
const int alturaOjos = 15;
const int pupilDetUmb = 20;
const char costado[] = "Left Eye";

//Sobel
const int scale = 1;
const int delta = 0;
const int ddepth = CV_16S;

//Posicion de las pantallas
const int gaussBlurx = 0;
const int gaussBlury = 350;
const int sobelx = 320;
const int sobely = 350;

const int iluminacion = 0x00000030L;
/* Posibles
 * 0xFFFFFFFF
 * 0x00000040L
 * 0x00000030L
 * 0x00000010L
 * 0x00000010L
 * 0x00000040L
 * 0x00000020L
 * 0x00000010L
 * 0x00000030L
 */
const bool rs232=false;
const int intensidad=30;


#endif
