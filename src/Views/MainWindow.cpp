// MainWindow.cpp
#include "../headerFiles/Views/MainWindow.h"
#include "../headerFiles/Views/DifficultyWindow.h"
#include "../headerFiles/Views/ProfileList.h"
#include "../headerFiles/Views/RulesDialogue.h"
#include "../headerFiles/Views/MainMenu.h"
#include "../headerFiles/Views/DemineurView.h"
#include "../headerFiles/Views/LeaderboardView.h"
#include <QStackedWidget>

#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QSlider>
#include <QWidgetAction>
#include <QMenuBar>
#include <QStandardPaths>
#include <QRandomGenerator>
#include <QDir>
#include <QTime>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Création du menu principal
    mainMenu = new MainMenu(this);

    QString profilesPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/profiles";
    QDir().mkpath(profilesPath);

    // navbar

    navbar = new QWidget(this);
    navbar->setFixedHeight(50);
    navbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    navbar->setContentsMargins(0, 0, 0, 0);
    navbar->setObjectName("navbar");
    navbar->setStyleSheet("QWidget#navbar { background-color: rgb(50, 50, 50); }");
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    QPushButton *homeButton = new QPushButton("Accueil", navbar);
    homeButton->setStyleSheet("QPushButton { background-color: transparent; border: none; color: white; }");

    QMenu *settingsMenu = new QMenu("Paramètres", navbar);
    settingsMenu->setStyleSheet("QMenu { background-color: rgb(50, 50, 50); color: white; }");

    settingsMenu->adjustSize();
    QPushButton *helpButton = new QPushButton("Aide", navbar);
    helpButton->setStyleSheet("QPushButton { background-color: transparent; border: none; color: white; }");

    navbarLayout->addWidget(homeButton);
    navbarLayout->addWidget(settingsMenu);
    navbarLayout->addWidget(helpButton);







    // Définir la navbar comme barre de menus
    setMenuWidget(navbar);

    connect(homeButton, &QPushButton::clicked, this, &MainWindow::openMainWindow);
    connect(helpButton, &QPushButton::clicked, []() {
        RulesDialog dialog;
        dialog.exec();
    });


    // Créer le menu déroulant "Paramètres"

    // Créer l'action "Mute"
    muteAction = new QAction("Mute", this);
    muteAction->setCheckable(true);
    muteAction->setChecked(false);
    QObject::connect(muteAction, &QAction::triggered, this, &MainWindow::toggleMute);
    settingsMenu->addAction(muteAction);

    // Créer le contrôle du volume
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    QObject::connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::setVolume);

    nextSong = new QAction("Next music", this);
    QObject::connect(nextSong, &QAction::triggered, [this]() {
        playlist->next();
    });

    prevSong = new QAction("Previous music", this);
    QObject::connect(prevSong, &QAction::triggered, [this]() {
        playlist->previous();
    });

    randomSong = new QAction("Random music", this);
    QObject::connect(randomSong, &QAction::triggered, [this]() {
        playlist->setCurrentIndex(QRandomGenerator::global()->bounded(playlist->mediaCount()));
    });

    settingsMenu->addAction(nextSong);
    settingsMenu->addAction(prevSong);
    settingsMenu->addAction(randomSong);




    // Créer une action pour le curseur de volume
    QWidgetAction *volumeAction = new QWidgetAction(this);
    volumeAction->setDefaultWidget(volumeSlider);

    // Ajouter l'action du curseur de volume au menu déroulant
    settingsMenu->addAction(volumeAction);

    QMenuBar *menuBar = new QMenuBar;
    menuBar->addMenu(settingsMenu);

    navbarLayout->addWidget(menuBar);
    

    // Widget central
    stackedWidget = new QStackedWidget;
    difficultyWindow = new DifficultyWindow;
    profileManager = new ProfileManager;
    profileList = new ProfileList(profileManager);
    leaderboardView = new LeaderboardView(profileManager);
    stackedWidget->addWidget(mainMenu);
    stackedWidget->addWidget(difficultyWindow);
    stackedWidget->addWidget(profileList);
    stackedWidget->addWidget(leaderboardView);

    setCentralWidget(stackedWidget);

    // Taille de la fenêtre
    resize(400, 300);

    // Musique de fond

    playlist = new QMediaPlaylist(this);
    playlist->addMedia(QUrl("qrc:/music/menuLoop.mp3"));
    playlist->addMedia(QUrl("qrc:/music/BackOnTrack.mp3"));
    playlist->addMedia(QUrl("qrc:/music/BaseAfterBase.mp3"));
    playlist->addMedia(QUrl("qrc:/music/Deadlocked.mp3"));
    playlist->addMedia(QUrl("qrc:/music/DryOut.mp3"));
    playlist->addMedia(QUrl("qrc:/music/Electrodynamix.mp3"));
    playlist->addMedia(QUrl("qrc:/music/Jumper.mp3"));
    playlist->addMedia(QUrl("qrc:/music/let-the-games-begin-21858.mp3"));
    playlist->addMedia(QUrl("qrc:/music/neon-gaming-128925.mp3"));
    playlist->addMedia(QUrl("qrc:/music/Polargeist.mp3"));
    playlist->addMedia(QUrl("qrc:/music/StereoMadness.mp3"));
    playlist->addMedia(QUrl("qrc:/music/xStep.mp3"));


    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    music = new QMediaPlayer(this);
    music->setPlaylist(playlist);
    music->play();

    // Connexion des signaux et des slots
    QObject::connect(mainMenu, &MainMenu::playClicked, this, &MainWindow::openDifficultyWindow);
    QObject::connect(profileList, &ProfileList::playClicked, this, &MainWindow::openDifficultyWindow);
    QObject::connect(mainMenu, &MainMenu::profileClicked, this, &MainWindow::openProfileWindow);
    QObject::connect(mainMenu, &MainMenu::leaderboardClicked, this, &MainWindow::openLeaderboardWindow);


    QObject::connect(difficultyWindow, &DifficultyWindow::demineurViewRequested, this, &MainWindow::openDemineurPageWithDifficulty);
    QObject::connect(difficultyWindow, &DifficultyWindow::demineurViewRequestedWithFile, this, &MainWindow::openDemineurPageWithFilePATH);
}

MainWindow::~MainWindow()
{
    delete playlist;
    delete music;
    delete mainMenu;
    delete difficultyWindow;
    delete profileList;
    delete stackedWidget;


}


void MainWindow::openDemineurPageWithDifficulty(int rows, int cols, int mines) {
    // Créez une instance de DemineurPage en fonction de la difficulté choisie
    DemineurView *newDemineurPage = nullptr ;
    if(profileManager->getProfileSelected())

    {

        QTime time = profileManager->curentProfile->getRecord(rows, cols, mines);

         newDemineurPage = new DemineurView(rows, cols, mines, time , this);
    }
    else
    {
         newDemineurPage = new DemineurView(rows, cols, mines, this);
    }

    QObject::connect(newDemineurPage, &DemineurView::BackToMain, this, &MainWindow::BackToMainPage);

    QObject::connect(newDemineurPage, &DemineurView::endGameRequest, this, &MainWindow::endGame);

    stackedWidget->addWidget(newDemineurPage);
    stackedWidget->setCurrentWidget(newDemineurPage);
}

void MainWindow::openDemineurPageWithFilePATH(QString filePath) {
    // Créez une instance de DemineurPage en fonction du fichier de sauvegarde


    DemineurView *newDemineurPage = nullptr ;
    if(profileManager->getProfileSelected())

    {

        newDemineurPage = new DemineurView(filePath, this);

        Demineur *demineur = newDemineurPage->getDemineur();        
        int cols = demineur->getNbColonnes();
        int rows = demineur->getNbLignes(); 
        int mines = demineur->getNbMines();
        QTime time = profileManager->curentProfile->getRecord(rows, cols, mines);
        newDemineurPage->setTime(time);

    }
    else{
        newDemineurPage = new DemineurView(filePath, this);
    }
    
    stackedWidget->addWidget(newDemineurPage);
    stackedWidget->setCurrentWidget(newDemineurPage);

}

void MainWindow::BackToMainPage()
{
    stackedWidget->setCurrentWidget(mainMenu);
}


void MainWindow::openDifficultyWindow()
{
    stackedWidget->setCurrentWidget(difficultyWindow);
    // mettre une music aléatoire
    playlist->setCurrentIndex(QRandomGenerator::global()->bounded(playlist->mediaCount()));
}

void MainWindow::openProfileWindow()
{
    stackedWidget->setCurrentWidget(profileList);
    playlist->setCurrentIndex(QRandomGenerator::global()->bounded(playlist->mediaCount()));

}

void MainWindow::openLeaderboardWindow()
{
    stackedWidget->setCurrentWidget(leaderboardView);
    playlist->setCurrentIndex(QRandomGenerator::global()->bounded(playlist->mediaCount()));
}

void MainWindow::openMainWindow()
{
    stackedWidget->setCurrentWidget(mainMenu);
    playlist->setCurrentIndex(0); 
}

void MainWindow::toggleMute()
{
    music->setMuted(muteAction->isChecked());
}

void MainWindow::setVolume(int volume)
{
    music->setVolume(volume);

}

void MainWindow::endGame(int rows , int cols, int mines, QTime time, bool win){
    profileManager->addGame(rows, cols, mines,time, win);
    
}






