# data mining algorithms for data preprocessing - data reduction 

This is a data mining project consisted of three different parts. The first part(NormalizeValues.c) has to do with data normalization. Hence it takes as input two csv files, representing two different datasets, and for each of them applies a simple normalization technique so to make each column contains values between [0,1]. 
#################################################################################################################


The next two parts aims at data reduction.

ENN.c is an implementation of the Edited Nearest Neighbors (ENN) algorithm, which is used in data mining for noise reduction in classification datasets. Assume we have the training set (TS) and an empty Edited Set (ES). We traverse the entire TS, and for each data item, we find its K nearest neighbors by computing a predefined metric (e.g., Euclidean distance). If the class label that occurs most frequently among the nearest neighbors of a data instance X is different from X's class label, then X is not added to the ES. Otherwise, we add X to the Edited Set (ES). After processing all the instances in the TS, we create a new dataset containing fewer instances than the initial one. This new dataset (ES) is used as input for a classification algorithm. By eliminating the outliers from the initial dataset, we expect better parameter definition during the training of the classification model and higher precision, recall, and F1-score values when computed on the test set.

####################################################################################################################################################################
