#ifndef NETWORK_H
#define NETWORK_H

#include <errno.h>
#include <time.h>
#include "Layer.h"
#include "Cost.h"
#include <stdlib.h>

#define OPT_EPS 10E-8

typedef struct Network Network;
struct Network
{
	ui nbLayers, currentLayer;
	Layer *layers;
};

typedef struct Optimizer Optimizer;
struct Optimizer
{
    ui iter;
    ld **Mwt, **Vwt, **Mbt, **Vbt;
};

typedef struct NNParam NNParam;
struct NNParam
{
    ui nbLayer, toLoopTrain, toLoopValidate, epoch,
    epochInterval, iSize, oSize, track, currH;
    ld l_rate, toExceed;
    ld l1Norm, l2Norm;
    ld **inputTrain, **outputTrain;
    ld **inputTest, **outputTest;
    char *cost_func, *StatsFile, *NNName, *endLayerAct;
    char *trainingFile, *validationFile;
    char **act_funcs;
    ui *hNeurons;
    Optimizer *optimizer;
};

void Network_Load(Network *net, char path[]);
void Network_Init(Network *net, cui nbLayers);
void Network_AddLayer(Network *net, Layer *layer);
void Network_Save(Network *net, char name[]);
void Network_Purge(Network *net);
Network *Network_DeepCopy(Network *net);
void Network_Display(Network *net, bool display_matr);
void Network_Wire(Network *net);
float *Network_Predict(Network *net, ld *input, cui Size);
ld *Network_Validate(Network *net, ld *input, cui Size, bool os1);
void Network_Train(Network *net, NNParam *params);
static void Network_Forward(Network *net, ld *input, cui iSize);
static ld Network_BackProp(Network *net, NNParam *params, cui nth);
static void IntegrityCheck(Network *net);
Layer *lvec_alloc(cui n);

void Optimizer_Init(Network *net, Optimizer *optz);
void Optimizer_Dispose(Network *net, Optimizer *optz, bool full);

#endif // NETWORK_H
