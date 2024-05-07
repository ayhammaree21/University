
import pandas as pd

def ReadTrainingData():
    nrows = int(pd.read_csv('PositiveTweets.tsv', sep='\\t', header=None,engine='python',encoding='UTF-8').shape[0])
    dfP = pd.read_csv('PositiveTweets.tsv', nrows=nrows, sep='\\t', header=None,engine='python')
    nrows = int(pd.read_csv('NegativeTweets.tsv', sep='\\t', header=None,engine='python').shape[0])
    dfN = pd.read_csv('NegativeTweets.tsv', nrows=nrows, sep='\\t', header=None,engine='python')

    AllTrainingData = pd.concat([dfN, dfP], ignore_index=True)

    AllTrainingData.columns = ['Class', 'Tweets']

    AllTrainingData.to_csv('TrainingData.csv',encoding='UTF-8', index=False)

ReadTrainingData()