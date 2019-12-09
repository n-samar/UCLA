"""
Author      : Yi-Chieh Wu, Sriram Sankararman
Description : Twitter
"""

from string import punctuation

import numpy as np

# !!! MAKE SURE TO USE SVC.decision_function(X), NOT SVC.predict(X) !!!
# (this makes ``continuous-valued'' predictions)
from sklearn.svm import SVC
from sklearn.cross_validation import StratifiedKFold
from sklearn import metrics

######################################################################
# functions -- input/output
######################################################################

def read_vector_file(fname):
    """
    Reads and returns a vector from a file.
    
    Parameters
    --------------------
        fname  -- string, filename
        
    Returns
    --------------------
        labels -- numpy array of shape (n,)
                    n is the number of non-blank lines in the text file
    """
    return np.genfromtxt(fname)


######################################################################
# functions -- feature extraction
######################################################################

def extract_words(input_string):
    """
    Processes the input_string, separating it into "words" based on the presence
    of spaces, and separating punctuation marks into their own words.
    
    Parameters
    --------------------
        input_string -- string of characters
    
    Returns
    --------------------
        words        -- list of lowercase "words"
    """
    
    for c in punctuation :
        input_string = input_string.replace(c, ' ' + c + ' ')
    return input_string.lower().split()


def extract_dictionary(infile):
    """
    Given a filename, reads the text file and builds a dictionary of unique
    words/punctuations.
    
    Parameters
    --------------------
        infile    -- string, filename
    
    Returns
    --------------------
        word_list -- dictionary, (key, value) pairs are (word, index)
    """
    
    word_list = {}
    with open(infile, 'rU') as fid :
        ### ========== TODO : START ========== ###
        # part 1a: process each line to populate word_list
        dict_len = len(word_list.keys())
        for line in fid:
            words = extract_words(line)
            for word in words:
                if word not in word_list.keys():
                    word_list[word] = dict_len
                    dict_len+=1
        ### ========== TODO : END ========== ###

    return word_list


def extract_feature_vectors(infile, word_list):
    """
    Produces a bag-of-words representation of a text file specified by the
    filename infile based on the dictionary word_list.
    
    Parameters
    --------------------
        infile         -- string, filename
        word_list      -- dictionary, (key, value) pairs are (word, index)
    
    Returns
    --------------------
        feature_matrix -- numpy array of shape (n,d)
                          boolean (0,1) array indicating word presence in a string
                            n is the number of non-blank lines in the text file
                            d is the number of unique words in the text file
    """
    
    num_lines = sum(1 for line in open(infile,'rU'))
    num_words = len(word_list)
    feature_matrix = np.zeros((num_lines, num_words))
    
    with open(infile, 'rU') as fid :
        ### ========== TODO : START ========== ###
        # part 1b: process each line to populate feature_matrix
        curr_tweet = 0
        for tweet in open(infile, 'rU'):
            words_in_tweet = extract_words(tweet)
            for word in words_in_tweet:
                feature_matrix[curr_tweet][word_list[word]] = 1
            curr_tweet+=1
        ### ========== TODO : END ========== ###
        
    return feature_matrix


######################################################################
# functions -- evaluation
######################################################################

def performance(y_true, y_pred, metric="accuracy"):
    """
    Calculates the performance metric based on the agreement between the 
    true labels and the predicted labels.
    
    Parameters
    --------------------
        y_true -- numpy array of shape (n,), known labels
        y_pred -- numpy array of shape (n,), (continuous-valued) predictions
        metric -- string, option used to select the performance measure
                  options: 'accuracy', 'f1-score', 'auroc'       
    
    Returns
    --------------------
        score  -- float, performance score
    """
    # map continuous-valued predictions to binary labels
    y_label = np.sign(y_pred)
    y_label[y_label==0] = 1
    score = 0
    ### ========== TODO : START ========== ###
    # part 2a: compute classifier performance
    if metric == 'accuracy':
        score = metrics.accuracy_score(y_true, y_label)
    elif metric == 'f1-score':
        score = metrics.f1_score(y_true, y_label)        
    elif metric == 'auroc':
        score = metrics.roc_auc_score(y_true, y_label)
    return score
    ### ========== TODO : END ========== ###


def cv_performance(clf, X, y, kf, metric="accuracy"):
    """
    Splits the data, X and y, into k-folds and runs k-fold cross-validation.
    Trains classifier on k-1 folds and tests on the remaining fold.
    Calculates the k-fold cross-validation performance metric for classifier
    by averaging the performance across folds.
    
    Parameters
    --------------------
        clf    -- classifier (instance of SVC)
        X      -- numpy array of shape (n,d), feature vectors
                    n = number of examples
                    d = number of features
        y      -- numpy array of shape (n,), binary labels {1,-1}
        kf     -- cross_validation.KFold or cross_validation.StratifiedKFold
        metric -- string, option used to select performance measure
    
    Returns
    --------------------
        score   -- float, average cross-validation performance across k folds
    """
    
    ### ========== TODO : START ========== ###
    # part 2b: compute average cross-validation performance
    sum = 0
    for train_index, test_index in kf:
        X_train, X_test = X[train_index], X[test_index]
        y_train, y_test = y[train_index], y[test_index]
        clf.fit(X_train, y_train)
        y_pred = clf.decision_function(X_test)
        sum += performance(y_test, y_pred, metric=metric)        
    return sum / kf.n_folds
    ### ========== TODO : END ========== ###


def select_param_linear(X, y, kf, metric="accuracy"):
    """
    Sweeps different settings for the hyperparameter of a linear-kernel SVM,
    calculating the k-fold CV performance for each setting, then selecting the
    hyperparameter that 'maximize' the average k-fold CV performance.
    
    Parameters
    --------------------
        X      -- numpy array of shape (n,d), feature vectors
                    n = number of examples
                    d = number of features
        y      -- numpy array of shape (n,), binary labels {1,-1}
        kf     -- cross_validation.KFold or cross_validation.StratifiedKFold
        metric -- string, option used to select performance measure
    
    Returns
    --------------------
        C -- float, optimal parameter value for linear-kernel SVM
    """
    
    print 'Linear SVM Hyperparameter Selection based on ' + str(metric) + ':'
    C_range = 10.0 ** np.arange(-3, 3)
    
    ### ========== TODO : START ========== ###
    # part 2: select optimal hyperparameter using cross-validation
    scores = []
    max_C = 0.001
    max_score = 0
    for C in [0.001, 0.01, 0.1, 1, 10, 100]:
        clf = SVC(C=C, kernel='linear')
        score = cv_performance(clf, X, y, kf, metric=metric)
        print("C: " + str(C) + ", score: " + str(score))
        if max_score < score:
            max_score = score
            max_C = C
    
    
    return max_C
    ### ========== TODO : END ========== ###



def performance_test(clf, X, y, metric="accuracy"):
    """
    Estimates the performance of the classifier using the 95% CI.
    
    Parameters
    --------------------
        clf          -- classifier (instance of SVC)
                          [already fit to data]
        X            -- numpy array of shape (n,d), feature vectors of test set
                          n = number of examples
                          d = number of features
        y            -- numpy array of shape (n,), binary labels {1,-1} of test set
        metric       -- string, option used to select performance measure
    
    Returns
    --------------------
        score        -- float, classifier performance
    """

    ### ========== TODO : START ========== ###
    # part 3: return performance on test data by first computing predictions and then calling performance

    score = 0
    y_pred = clf.predict(X)
    return performance(y, y_pred, metric)
    ### ========== TODO : END ========== ###


######################################################################
# main
######################################################################
 
def main() :
    np.random.seed(1234)
    
    # read the tweets and its labels   
    dictionary = extract_dictionary('../data/tweets.txt')
    X = extract_feature_vectors('../data/tweets.txt', dictionary)
    y = read_vector_file('../data/labels.txt')
    
    metric_list = ["accuracy", "f1_score", "auroc"]
    
    ### ========== TODO : START ========== ###
    # part 1: split data into training (training + cross-validation) and testing set
    X_train = X[:560]
    X_test = X[560:]
    print(X.shape)
    y_train = y[:560]
    y_test = y[560:]
    # part 2: create stratified folds (5-fold CV)
    kf = StratifiedKFold(y_train, n_folds=5)
    # part 2: for each metric, select optimal hyperparameter for linear-kernel SVM using CV
    C_max_accuracy = select_param_linear(X, y, kf, metric="accuracy")
    print(C_max_accuracy)
    C_max_f1_score = select_param_linear(X, y, kf, metric="f1-score")
    print(C_max_f1_score)
    C_max_auroc = select_param_linear(X, y, kf, metric="auroc")    
    print(C_max_auroc)

    # part 3: train linear-kernel SVMs with selected hyperparameters
    clf = SVC(C=C_max_accuracy, kernel='linear')
    clf.fit(X_train, y_train)
    # part 3: report performance on test data
    print("Accuracy performance: " + str(performance_test(clf, X_test, y_test, metric='accuracy')))
    print("F1-score performance: " + str(performance_test(clf, X_test, y_test, metric='f1-score')))
    print("AUROC performance: " + str(performance_test(clf, X_test, y_test, metric='auroc')))    
    ### ========== TODO : END ========== ###
    
    
if __name__ == "__main__" :
    main()