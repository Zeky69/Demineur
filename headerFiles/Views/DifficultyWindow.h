// DifficultyWindow.h

#ifndef DIFFICULTYWINDOW_H
#define DIFFICULTYWINDOW_H

#include <QWidget>
#include <QLineEdit>


// Ajoutez l'inclusion nécessaire pour DemineurView si elle n'est pas déjà incluse
#include "DemineurView.h"

class DifficultyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DifficultyWindow(QWidget *parent = nullptr);
    void launchDemineurView(int rows, int cols, int mines);
    void loadGame(QString filePath);
    bool validateInput(QLineEdit *rowsLineEdit, QLineEdit *colsLineEdit, QLineEdit *minesLineEdit);




signals:
    void demineurViewRequested(int rows, int cols, int mines);
    void demineurViewRequestedWithFile(QString filePath);
    void endGameRequest(int rows , int cols, int mines, QTime time, bool wind);

    


private slots:
    void showCustomDialog();



private:
    // Déclarez la fonction de lancement dans la section privée pour qu'elle soit utilisable dans la classe

};

#endif // DIFFICULTYWINDOW_H
