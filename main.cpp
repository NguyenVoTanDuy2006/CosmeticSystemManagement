#include "productManager.h"
#include "TradeManager.h"

int main(int argc, char *argv[])
{
    // // Test product management
    productManager pm;
    productInfo info1("Sua Rua Mat", "Hada Labo", TYPE::SUA_RUA_MAT, UNIT::TUYT, 150000);
    pm.addProduct(info1);
    pm.displayAll();
    pm.saveData();

    qDebug() << "\n--- Reading data back ---";

    // Test trade management
    QString testData = "SP00001\n2024/06/27 23:00:00\n100\n100000\nNVTD\n0917957481\nTan Phu\n";
    QTextStream ss(&testData);

    auto trade = TradeFactory::getTrade(QStringLiteral("some_id_1")); // Type 1 for OUT
    if (trade) {
        trade->readData(ss);

        QTextStream cout(stdout);
        trade->writeData(cout);
        cout << Qt::endl;
    }

    return 0;
}