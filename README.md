# data mining algorithms for data preprocessing - data reduction in C 

This is a data mining project divided in three parts.  Below is a description of the individual components:

1st: Data Normalization (NormalizeValues.c)

This module takes as input two CSV files, each representing a dataset. For each dataset, it applies a simple normalization technique to ensure that all column values are scaled to the range [0,1]. This preprocessing step is essential for improving the performance of machine learning models by ensuring consistent feature scaling.

################################################################################################################################################################################

2nd: Noise Reduction(ENN.c)

ENN.c is an implementation of the Edited Nearest Neighbors (ENN) algorithm, which is used in data mining for noise reduction in classification datasets. The process starts with the Training Set(TS) and an empty Edited Set(ES). We traverse the entire TS, and for each data item, we find its K nearest neighbors by computing a predefined metric (e.g., Euclidean distance). If the class label that occurs most frequently among the nearest neighbors of a data instance X is different from X's class label, then X is not added to the ES. Otherwise, we add X to the Edited Set (ES). After processing all the instances in the TS, we create a new dataset containing fewer instances than the initial one. This new dataset (ES) is used as input for a classification algorithm. By eliminating the outliers from the initial dataset, we expect better parameter definition during the training of the classification model and higher precision, recall, and F1-score values when evaluated on the test set.

As mentioned above the ES starts empty and data instances are added to it during processing. The standard ENN algorithm initializes the Edited Set as a full copy of the Training Set TS and removes from the ES the instances whose  class labels differ from the representative class label of their nearest neighbors in the TS. While this implementation requires more hands-on work, especially in the part of  reading/writing from/to .csv files , it produces the exact same output. Therefore, I chose to use the simpler approach described above.

####################################################################################################################################################################

3rd: Data Reduction(IB2.c)

IB2 begins by creating an empty condensing set(CS). Its goal is to end up with a CS containing only the representatives of the original dataset. Simply put, if a data item lies in an area that its nearest neighbor is of the same class, then the neighbor can represent it, and there is no need to keep both of them in the dataset. As a first step IB2 selects a data item randomly from the training set and moves it to the CS. Next it traverses the TS  and for each data item it finds it's nearest neighbor in the CS based on a specific metric. If the nearest neighbor's class is different than the TS item's class, then the current item is moved from the TS to the CS. The resulting CS depends on the first item chosen randomly from the TS and was put in the CS. This CS is used as a training set in Data Mining/ML model instead of the actual one.
