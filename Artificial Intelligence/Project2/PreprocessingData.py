import pickle
import pandas as pd
from sklearn.svm import SVC
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from CleanData import clean_tweet
from CleanData import set_emoji_feature
from sklearn.metrics import confusion_matrix
import seaborn as sns
from sklearn.model_selection import StratifiedKFold
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import MultinomialNB

def preproccess():
    data = pd.read_csv('TrainingData.csv', index_col=False)
    data = set_emoji_feature(data)
    data['Tweets'] = data['Tweets'].apply(clean_tweet)
    data.to_csv('CleanedData.csv', encoding='UTF-8', index=False)
    return data

def extract_features(data):

    # Extract features from the tweet text using the CountVectorizer method
    vectorizer = CountVectorizer(max_features=10000,token_pattern=r'[^\s]+',ngram_range=(1, 2))

    input_data = data['Tweets'].astype(str) + ' ' + data['Emoji'].astype(str)
    features = vectorizer.fit_transform(input_data)

    class_ = data['Class']
    number_of_features = len(vectorizer.vocabulary_)

    # Save the vectorizers to disk
    with open('vectorized.pickle', 'wb') as handle:
        pickle.dump(vectorizer, handle)

    return features,class_,number_of_features



def Naive_Bayes(X_train, X_test, y_train, y_test ):

    #Train and evaluate the Naive Bayes classifier
    nb_clf = MultinomialNB()
    nb_clf.fit(X_train, y_train)
    y_pred = nb_clf.predict(X_test)

    # Save the trained model to disk
    with open('naive_bayes_model.pickle', 'wb') as handle:
        pickle.dump(nb_clf, handle)

    cm = confusion_matrix(y_test, y_pred)
    print("Confusion Matrix:\n", cm)

    # plot confusion matrix
    heatmap= sns.heatmap(cm, annot=True, cmap='Blues', fmt='g')
    heatmap.set_title('Confusion Matrix For Naive Bayes Classifier')

    nb_accuracy = accuracy_score(y_test, y_pred)
    nb_precision = precision_score(y_test, y_pred, pos_label='pos')
    nb_recall = recall_score(y_test, y_pred, pos_label='pos')
    nb_f1 = f1_score(y_test, y_pred, pos_label='pos')

    print("Naive Bayes Classifier:")
    print("Accuracy:", nb_accuracy)
    print("Precision:", nb_precision)
    print("Recall:", nb_recall)
    print("F1-Score:", nb_f1)
    plt.show()
    #plot(nb_accuracy, nb_precision, nb_recall, nb_f1,'Naive Bayes Classifier Results')

def Decision_Tree(X_train, X_test, y_train, y_test ):
    # Train and evaluate the Decision Tree classifier

    dt_clf = DecisionTreeClassifier()
    dt_clf.fit(X_train, y_train)
    y_pred = dt_clf.predict(X_test)

    # Save the trained model to disk
    with open('decision_tree_model.pickle', 'wb') as handle:
        pickle.dump(dt_clf, handle)

    cm = confusion_matrix(y_test, y_pred)
    print("Confusion Matrix:\n", cm)

    # plot confusion matrix
    heatmap = sns.heatmap(cm, annot=True, cmap='Blues', fmt='g')
    heatmap.set_title('Confusion Matrix For Decision Tree Classifier')

    dt_accuracy = accuracy_score(y_test, y_pred)
    dt_precision = precision_score(y_test, y_pred, pos_label='pos')
    dt_recall = recall_score(y_test, y_pred, pos_label='pos')
    dt_f1 = f1_score(y_test, y_pred, pos_label='pos')

    print("Decision Tree classifier:")
    print("Accuracy:", dt_accuracy)
    print("Precision:", dt_precision)
    print("Recall:", dt_recall)
    print("F1-Score:", dt_f1)
    plt.show()
    #plot(dt_accuracy, dt_precision, dt_recall, dt_f1, 'Decision Tree Classifier Results')



def Random_Forest(X_train, X_test, y_train, y_test ):
    # create and train the random forest classifier
    rf_clf = RandomForestClassifier(n_estimators=100, random_state=42)
    rf_clf.fit(X_train, y_train)

    # Save the trained model to disk
    with open('random_forest_model.pickle', 'wb') as handle:
        pickle.dump(rf_clf, handle)

    # predict on the test set
    y_pred = rf_clf.predict(X_test)

    cm = confusion_matrix(y_test, y_pred)
    print("Confusion Matrix:\n", cm)

    # plot confusion matrix
    heatmap = sns.heatmap(cm, annot=True, cmap='Blues', fmt='g')
    heatmap.set_title('Confusion Matrix For Random Forest Classifier')

    # calculate evaluation metrics
    rf_accuracy = accuracy_score(y_test, y_pred)
    rf_precision = precision_score(y_test, y_pred, pos_label='pos')
    rf_recall = recall_score(y_test, y_pred, pos_label='pos')
    rf_f1 = f1_score(y_test, y_pred, pos_label='pos')

    # print the evaluation metrics
    print("Random Forest classifier:")
    print("Accuracy:", rf_accuracy)
    print("Precision:", rf_precision)
    print("Recall:", rf_recall)
    print("F1-Score:", rf_f1)
    plt.show()
    # plot(rf_accuracy, rf_precision, rf_recall, rf_f1, 'Random Forest Classifier Results')


def Support_Vector_Machines(X_train, X_test, y_train, y_test):

        svm_model = SVC(kernel='linear', C=1, gamma='auto')
        # Evaluate SVM model on testing data
        svm_model.fit(X_train, y_train)

        # Save the trained model to disk
        with open('Support_Vector_Machines.pickle', 'wb') as handle:
            pickle.dump(svm_model, handle)

        # predict on the test set
        y_pred = svm_model.predict(X_test)

        cm = confusion_matrix(y_test, y_pred)
        print("Confusion Matrix:\n", cm)
        # plot confusion matrix
        heatmap = sns.heatmap(cm, annot=True, cmap='Blues', fmt='g')
        heatmap.set_title('Confusion Matrix For Support Vector Machines Classifier')
        # calculate evaluation metrics
        nn_accuracy = accuracy_score(y_test, y_pred)
        nn_precision = precision_score(y_test, y_pred, pos_label='pos')
        nn_recall = recall_score(y_test, y_pred, pos_label='pos')
        nn_f1 = f1_score(y_test, y_pred, pos_label='pos')

        # print the evaluation metrics
        print("Support Vector Machines Classifier:")
        print("Accuracy:", nn_accuracy)
        print("Precision:", nn_precision)
        print("Recall:", nn_recall)
        print("F1-Score:", nn_f1)
        plt.show()
       # plot(nn_accuracy, nn_precision, nn_recall, nn_f1, 'Support Vector Machines Classifier Results')



def Five_foldes_cross_validation(features, class_):

    # Create a StratifiedKFold object
    skf = StratifiedKFold(n_splits=5)
    svm_model = SVC(kernel='linear', C=1, gamma='auto')

    accuracy_scores = []
    precision_scores = []
    recall_scores = []
    f1_scores = []

    # create an empty confusion matrix
    cm = np.zeros((2, 2))
    for train_index, test_index in skf.split(features, class_):

        X_train, X_test = features[train_index], features[test_index]
        y_train, y_test = class_[train_index], class_[test_index]

        # Evaluate SVM model on testing data
        svm_model.fit(X_train, y_train)

        # predict on the test set
        y_pred = svm_model.predict(X_test)

        # calculate the confusion matrix for this fold
        fold_cm = confusion_matrix(y_test, y_pred)
        cm += fold_cm
        # Calculate evaluation metrics
        accuracy_scores.append(accuracy_score(y_test, y_pred))
        precision_scores.append(precision_score(y_test, y_pred, pos_label='pos'))
        recall_scores.append(recall_score(y_test, y_pred, pos_label='pos'))
        f1_scores.append(f1_score(y_test, y_pred, pos_label='pos'))

    heatmap = sns.heatmap(cm, annot=True, cmap='Blues', fmt='g')
    heatmap.set_title('Confusion Matrix For Support Vector Machines Classifier')
    plt.show()
    print("accuracy:", np.mean(accuracy_scores))
    print("precision:", np.mean(precision_scores))
    print("recall:", np.mean(recall_scores))
    print("F1 score:", np.mean(f1_scores))
    #plot(np.mean(accuracy_scores), np.mean(precision_scores), np.mean(recall_scores), np.mean(f1_scores), "Naive Bayes Classifier Results")


def plot(accuracy, precision, recall, f1_score, title):
    metrics = ['Mean Accuracy', 'Mean Precision', 'Mean Recall', 'Mean F1-Score']
    values = [accuracy, precision, recall, f1_score]

    plt.bar(metrics, values)
    plt.title(title)
    plt.yticks([i / 10 for i in range(0, 11, 1)])
    # Add y-values to top of bars
    for i, v in enumerate(values):
        plt.text(i, v, str(round(v, 3)), ha='center', va='bottom')
    plt.show()



data = preproccess()
features,class_,number_of_features= extract_features(data)
print(number_of_features)

#Five_foldes_cross_validation(features, class_)

# Split the data into training and testing sets

#X_train, X_test, y_train, y_test = train_test_split(features, class_, test_size=0.25, random_state=42)

#Support_Vector_Machines(X_train, X_test, y_train, y_test)
#Naive_Bayes(X_train, X_test, y_train, y_test)
#Decision_Tree(X_train, X_test, y_train, y_test)
#Random_Forest(X_train, X_test, y_train, y_test)



































