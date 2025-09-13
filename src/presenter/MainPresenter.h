//
// Created by gawain on 9/11/25.
//

#ifndef TURNLAB_MAINPRESENTER_H
#define TURNLAB_MAINPRESENTER_H

#include <string>

#include "../view/MainWindow.h"
#include "DXFImportPresenter.h"



class MainPresenter {
    MainWindow window;

    std::unique_ptr<DXFImportPresenter> dxfImportPresenter;

public:
    MainPresenter();
    explicit MainPresenter(std::string inputDXF);

};


#endif //TURNLAB_MAINPRESENTER_H