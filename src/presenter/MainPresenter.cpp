//
// Created by gawain on 9/11/25.
//

#include "MainPresenter.h"

MainPresenter::MainPresenter() {
    window.show();
}

MainPresenter::MainPresenter(std::string inputDXF) {
    window.show();
    dxfImportPresenter = std::make_unique<DXFImportPresenter>(inputDXF, &window);
    dxfImportPresenter->showDialog();
}

