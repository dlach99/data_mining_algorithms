# data mining algorithms for data preprocessing - data reduction 

This is a data mining project divided in three parts.  Below is a description of the individual components:

1st: Data Normalization (NormalizeValues.c)
This module takes as input two CSV files, each representing a dataset. For each dataset, it applies a simple normalization technique to ensure that all column values are scaled to the range [0,1]. This preprocessing step is essential for improving the performance of machine learning models by ensuring consistent feature scaling.

################################################################################################################################################################################

2nd: Noise Reduction(ENN.c)
ENN.c is an implementation of the Edited Nearest Neighbors (ENN) algorithm, which is used in data mining for noise reduction in classification datasets. Assume we have the training set (TS) and an empty Edited Set (ES). We traverse the entire TS, and for each data item, we find its K nearest neighbors by computing a predefined metric (e.g., Euclidean distance). If the class label that occurs most frequently among the nearest neighbors of a data instance X is different from X's class label, then X is not added to the ES. Otherwise, we add X to the Edited Set (ES). After processing all the instances in the TS, we create a new dataset containing fewer instances than the initial one. This new dataset (ES) is used as input for a classification algorithm. By eliminating the outliers from the initial dataset, we expect better parameter definition during the training of the classification model and higher precision, recall, and F1-score values when computed on the test set.

As mentioned above ES is empty at the begining and data items are added to it. Actually ENN algorithm starts with an Edited Set which is a copy of the Training Set TS and removes from the ES the items have differet class label from the representative class label of their nearest neighbors in the TS. Although this implementation would more demanding, if we consider that we read and write from/to .csv files in C language, and it results exatctly in the same output.

####################################################################################################################################################################

3rd: Data Reduction(IB2.c)
IB2 starts with the creation of an empty condensing set(CS). Its purpose is to end up with a CS that contains only the representatives of the actual dataset. Simply put, if a data item lies in an area that its nearest neighbor is of the same class, then the neighbor can represent it, and there is no need to keep both of them in the dataset. As a first step IB2 selects a data item randomly from the training set and moves it to the CS. Next it traverses the TS  and for each data item it finds it's nearest neighbor in the CS based on a specific metric. If the nearest neighbor's class is different than the TS item's class, then the current item is moved from the TS to the CS. The resulting CS depends on the first item chosen randomly from the TS and was put in the CS. This CS is used as a training set in Data Mining/ML model instead of the actual one.
