#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "main.h"
#include "wallet.h"
#include "init.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include "pow_control.h"
#include "blockbrowser.h"
#include <sstream>
#include <string>

using namespace json_spirit;

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
    
    //resize(400, 420);
    
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerCountDown()));
    timer->start(1000);
    // connect(startButton, SIGNAL (released()), this, SLOT (updateStatistics()));

    // ui->phaselbl1->setText("Block 1 -> " + QString::number(LAST_POW_BLOCK) + ": PoW & PoS");
    // ui->phaselbl2->setText("Block " + QString::number(LAST_POW_BLOCK + 1) + "-> ... : PoS");

}

int heightPrevious = -1;
int connectionPrevious = -1;
int volumePrevious = -1;
double rewardPrevious = -1;
double netPawratePrevious = -1;
double pawratePrevious = -1;
double hardnessPrevious = -1;
double hardnessPrevious2 = -1;
int stakeminPrevious = -1;
int stakemaxPrevious = -1;
QString stakecPrevious = "";
int dec1 = 10;


void StatisticsPage::updateStatistics()
{
    double pHardness = GetDifficulty();
    double pHardness2 = GetDifficulty(GetLastBlockIndex(pindexBest, true));
    //int pPawrate = GetPoWMHashPS();
    double pPawrate2 = 0.000;
    int nHeight = pindexBest->nHeight;
    int pPawrate = getBlockHashrate(nHeight);
    double nSubsidy = 50;
    uint64_t nMinWeight = 0, nMaxWeight = 0, nWeight = 0;
    pwalletMain->GetStakeWeight(*pwalletMain, nMinWeight, nMaxWeight, nWeight);
    uint64_t nNetworkWeight = GetPoSKernelPS();
    int64_t volume = ((pindexBest->nMoneySupply)/100000000);
    int peers = this->model->getNumConnections();
    pPawrate2 = (double)pPawrate;
    QString height = QString::number(nHeight);
    QString stakemin = QString::number(nMinWeight);
    QString stakemax = QString::number(nNetworkWeight);
    QString phase = "";

    if (pindexBest->nHeight > LAST_POW_BLOCK)
        phase = "Proof of Stake";
    else
        phase = "Proof of Work / Proof of Stake";

    QString subsidy = QString::number(nSubsidy, 'f', 6);
    QString hardness = QString::number(pHardness, 'f', 6);
    QString hardness2 = QString::number(pHardness2, 'f', 6);
    QString pawrate = QString::number(pPawrate2, 'f', 0);
    QString Qlpawrate = model->getLastBlockDate().toString();

    QString QPeers = QString::number(peers);
    QString qVolume = QString::number(volume);

    if(nHeight > heightPrevious)
    {
        ui->heightBox->setText("<b><font color=\"yellow\">" + height + "</font></b>");
    } else {
    ui->heightBox->setText(height);
    }

    if(0 > stakeminPrevious)
    {
        ui->minBox->setText("<b><font color=\"yellow\">" + stakemin + "</font></b>");
    } else {
    ui->minBox->setText(stakemin);
    }
    if(0 > stakemaxPrevious)
    {
        ui->maxBox->setText("<b><font color=\"yellow\">" + stakemax + "</font></b>");
    } else {
    ui->maxBox->setText(stakemax);
    }

    if(phase != stakecPrevious)
    {
        ui->cBox->setText("<b><font color=\"yellow\">" + phase + "</font></b>");
    } else {
    ui->cBox->setText(phase);
    }

    
    if(nSubsidy < rewardPrevious)
    {
        ui->rewardBox->setText("<b><font color=\"red\">" + subsidy + "</font></b>");
    } else {
    ui->rewardBox->setText(subsidy);
    }
    
    if(pHardness > hardnessPrevious)
    {
        ui->diffBox->setText("<b><font color=\"yellow\">" + hardness + "</font></b>");
    } else if(pHardness < hardnessPrevious) {
        ui->diffBox->setText("<b><font color=\"red\">" + hardness + "</font></b>");
    } else {
        ui->diffBox->setText(hardness);        
    }

    if(pHardness2 > hardnessPrevious2)
    {
        ui->diffBox2->setText("<b><font color=\"yellow\">" + hardness2 + "</font></b>");
    } else if(pHardness2 < hardnessPrevious2) {
        ui->diffBox2->setText("<b><font color=\"red\">" + hardness2 + "</font></b>");
    } else {
        ui->diffBox2->setText(hardness2);
    }
    
    if(pPawrate2 > netPawratePrevious)
    {
        ui->pawrateBox->setText("<b><font color=\"yellow\">" + pawrate + " H/s</font></b>");
    } else if(pPawrate2 < netPawratePrevious) {
        ui->pawrateBox->setText("<b><font color=\"red\">" + pawrate + " H/s</font></b>");
    } else {
        ui->pawrateBox->setText(pawrate + " H/s");
    }

    if(Qlpawrate != pawratePrevious)
    {
        ui->localBox->setText("<b><font color=\"yellow\">" + Qlpawrate + "</font></b>");
    } else {
    ui->localBox->setText(Qlpawrate);
    }
    
    if(peers > connectionPrevious)
    {
        ui->connectionBox->setText("<b><font color=\"yellow\">" + QPeers + "</font></b>");
    } else if(peers < connectionPrevious) {
        ui->connectionBox->setText("<b><font color=\"red\">" + QPeers + "</font></b>");
    } else {
        ui->connectionBox->setText(QPeers);  
    }

    if(volume > volumePrevious)
    {
        ui->volumeBox->setText("<b><font color=\"yellow\">" + qVolume + " EGC" + "</font></b>");
    } else if(volume < volumePrevious) {
        ui->volumeBox->setText("<b><font color=\"red\">" + qVolume + " EGC" + "</font></b>");
    } else {
        ui->volumeBox->setText(qVolume + " EGC");
    }
    updatePrevious(nHeight, nMinWeight, nNetworkWeight, phase, nSubsidy, pHardness, pHardness2, pPawrate2, Qlpawrate, peers, volume);
}

void StatisticsPage::updatePrevious(int nHeight, int nMinWeight, int nNetworkWeight, QString phase, double nSubsidy, double pHardness, double pHardness2, double pPawrate2, QString Qlpawrate, int peers, int volume)
{
    heightPrevious = nHeight;
    stakeminPrevious = nMinWeight;
    stakemaxPrevious = nNetworkWeight;
    stakecPrevious = phase;
    rewardPrevious = nSubsidy;
    hardnessPrevious = pHardness;
    hardnessPrevious2 = pHardness2;
    netPawratePrevious = pPawrate2;
    pawratePrevious = Qlpawrate;
    connectionPrevious = peers;
    volumePrevious = volume;
}

void StatisticsPage::setModel(ClientModel *model)
{
    updateStatistics();
    this->model = model;
}


StatisticsPage::~StatisticsPage()
{
    delete ui;
}

void StatisticsPage::timerCountDown()
{
    dec1 = dec1 - 1;
    ui->startButton->setText("Update Statistics " + QString::number(dec1));
    if (dec1 == 0)
    {
        updateStatistics();
        dec1 = 10;
    }
}

void StatisticsPage::on_startButton_released()
{
    updateStatistics();
    dec1 = 11;
}
