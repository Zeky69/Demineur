#ifndef PROFILELIST_H
#define PROFILELIST_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QList>
#include <QLabel>
#include "Profile.h"
#include "ProfileManager.h" // Inclure le ProfileManager

class ProfileList : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileList(ProfileManager* profileManager, QWidget *parent = nullptr); // Ajouter ProfileManager en paramètre

    void loadProfiles(); // Méthode pour charger les profils existants

signals:
    void profileSelected(Profile *profile);
    void playClicked();


private slots:
    void handleProfileItemClicked();
    void handleAddProfileClicked();
    void handleDeleteProfileClicked();
    void addProfile(Profile *profile);
    void handlePlayButtonClicked();
    void deleteSelectedProfile();
    void updatePlayButtonState();
    Profile* getSelectedProfile();

private:
    QListWidget *profileListView;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *playButton;
    QLabel *avatarLabel;
    ProfileManager* profileManager;
    QList<Profile*> profiles;
};

#endif // PROFILELIST_H
