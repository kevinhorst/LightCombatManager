#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "FileHandler.hpp"

class QMessageBox;

class AdditionalSettings;
class CombatWidget;
class DirSettings;
class RuleSettings;
class WelcomeWidget;

// This class handles the creation and managing of the main GUI window. It also serves
// as "main anchor point", switching between LCM's widgets, which are created in other classes.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

signals:
    void
    setSaveAction(bool enable);

private slots:
    void
    newCombat();

    bool
    saveTable();

    void
    saveAs();

    void
    openTable();

    void
    openSettings();

    void
    about();

    void
    exitCombat();

private:
    void
    setWelcomingWidget();

    void
    setTableWidget(bool           isDataStored,
                   bool           newCombatStarted,
                   const QString& data = "");

    void
    setCombatTitle(bool isCombatActive);

    [[nodiscard]] int
    createSaveMessageBox(const QString& tableMessage,
                         bool           isClosing);

    [[nodiscard]] QString
    createRuleChangeMessageBoxText() const;

    void
    closeEvent(QCloseEvent *event);

    [[nodiscard]] bool
    checkStoredTableRules(QString data);

private:
    QPointer<WelcomeWidget> m_welcomeWidget;
    QPointer<CombatWidget> m_combatWidget;

    bool m_isTableActive{ false };
    bool m_tableInFile{ false };

    QString m_fileName{ "" };

    FileHandlerRef m_file;
    std::shared_ptr<AdditionalSettings> m_additionalSettings;
    std::shared_ptr<RuleSettings> m_ruleSettings;
    std::shared_ptr<DirSettings> m_dirSettings;

    RuleSettings::Ruleset m_loadedTableRule;
    bool m_loadedTableRollAutomatically;

    static constexpr int START_WIDTH = 720;
    static constexpr int START_HEIGHT = 240;

    static constexpr int COL_RULESET = 8;
    static constexpr int COL_ROLL_AUTOMATICALLY = 9;
};
#endif
