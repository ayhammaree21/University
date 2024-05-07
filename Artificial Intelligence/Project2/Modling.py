from CleanData import set_emoji_feature, clean_tweet
from MyClassifier import MyClassifier
from sklearn.feature_extraction.text import CountVectorizer
import pandas as pd

nb_model = MyClassifier('naive_bayes_model.pickle','vectorized.pickle')
dt_model = MyClassifier('decision_tree_model.pickle','vectorized.pickle')
rf_model = MyClassifier('random_forest_model.pickle','vectorized.pickle')
SVM_model = MyClassifier('Support_Vector_Machines.pickle','vectorized.pickle')

print("Enter the name of the file that contains the tweets: ",end='\n')
fileName = input(str())

while(1):

    TestData = pd.read_csv(fileName, index_col=False)

    print("1 - Naive Bayes Model", end='\n')
    print("2 - Decision Tree Model", end='\n')
    print("3 - Random Forest Model", end='\n')
    print("4 - Support Vector Machines", end='\n\n')
    model_option = num = int(input("Choose the model: "))
    TestData = set_emoji_feature(TestData)
    TestData['Tweets'] = TestData['Tweets'].apply(clean_tweet)
    predictions=[]

    if(model_option == 1):

        predictions = nb_model.predict(TestData['Tweets'] + TestData['Emoji'])
    elif (model_option == 2):

        predictions = dt_model.predict(TestData['Tweets'] + TestData['Emoji'])
    elif (model_option == 3):

        predictions = rf_model.predict(TestData['Tweets'] + TestData['Emoji'])
    elif (model_option == 4):

        predictions = SVM_model.predict(TestData['Tweets'] + TestData['Emoji'])

    for tweet, prediction in zip(TestData['Tweets'], predictions):
        print(f"Tweet: {tweet} \t Prediction: {prediction}")










