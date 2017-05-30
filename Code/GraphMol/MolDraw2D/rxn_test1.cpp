//
//  Copyright (C) 2017 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//

#include <RDGeneral/utils.h>
#include <RDGeneral/Invariant.h>
#include <RDGeneral/RDLog.h>
#include <GraphMol/RDKitBase.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/ChemReactions/ReactionParser.h>
#include <GraphMol/ChemReactions/SanitizeRxn.h>
#include <GraphMol/Depictor/RDDepictor.h>

#include <GraphMol/MolDraw2D/MolDraw2D.h>
#include <GraphMol/MolDraw2D/MolDraw2DSVG.h>
#include <GraphMol/MolDraw2D/MolDraw2DUtils.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace RDKit;

#ifdef RDK_CAIRO_BUILD
#include <cairo.h>
#include "MolDraw2DCairo.h"
#endif

namespace {
void drawit(ChemicalReaction *rxn, std::string nameBase,
            bool highlight_map = false) {
  double panex = 200, paney = 150;
  double width = panex * (rxn->getNumReactantTemplates() +
                          rxn->getNumProductTemplates() + 1);
  double height = paney;
#ifdef RDK_CAIRO_BUILD
  {
    MolDraw2DCairo drawer(width, height);
    drawer.drawReaction(*rxn, highlight_map);
    drawer.finishDrawing();
    drawer.writeDrawingText(nameBase + ".png");
  }
#endif
  {
    std::ofstream outs((nameBase + ".svg").c_str());
    MolDraw2DSVG drawer(width, height, outs);
    drawer.drawReaction(*rxn, highlight_map);
    drawer.finishDrawing();
    outs.flush();
  }
}
}

void test1() {
  std::cout << " ----------------- Test 1" << std::endl;
  {
    std::string smiles =
        "[CH3:1][C:2](=[O:3])[OH:4].[CH3:5][NH2:6]>CC(O)C.[Pt]>[CH3:1][C:2](=["
        "O:3])[NH:6][CH3:5].[OH2:4]";
    std::string nameBase = "rxn_test1_1";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase);
    delete rxn;
  }

  {
    std::string smiles =
        "[N:1][C:2][C:3](=[O:4])[O:5].[N:6][C:7][C:8](=[O:9])[O:10]>>[N:1]1[C:"
        "2][C:3](=[O:4])[N:6][C:7][C:8]1=[O:9].[O:5][O:10]";
    std::string nameBase = "rxn_test1_2";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase);
    delete rxn;
  }

  {
    std::string smiles =
        ">>[N:1]1[C:"
        "2][C:3](=[O:4])[N:6][C:7][C:8]1=[O:9].[O:5][O:10]";
    std::string nameBase = "rxn_test1_3";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase);
    delete rxn;
  }
  {
    std::string smiles =
        "[N:1][C:2][C:3](=[O:4])[O:5].[N:6][C:7][C:8](=[O:9])[O:10]>>";
    std::string nameBase = "rxn_test1_4";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase);
    delete rxn;
  }

  {
    std::string smiles =
        "[N:1][C:2][C:3](=[O:4])[O:5].[N:6][C:7][C:8](=[O:9])[O:10]>O.ClCl>";
    std::string nameBase = "rxn_test1_5";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase);
    delete rxn;
  }

  {
    std::string smiles =
        "[CH3:1][C:2](=[O:3])[OH:4].[CH3:5][NH2:6]>CC(O)C.[Pt]>[CH3:1][C:2](=["
        "O:3])[NH:6][CH3:5].[OH2:4]";
    std::string nameBase = "rxn_test1_6";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase, true);
    delete rxn;
  }

  {
    std::string smiles =
        "[N:1][C:2][C:3](=[O:4])[O:5].[N:6][C:7][C:8](=[O:9])[O:10]>>[N:1]1[C:"
        "2][C:3](=[O:4])[N:6][C:7][C:8]1=[O:9].[O:5][O:10]";
    std::string nameBase = "rxn_test1_7";
    bool useSmiles = true;
    ChemicalReaction *rxn =
        RxnSmartsToChemicalReaction(smiles, NULL, useSmiles);
    TEST_ASSERT(rxn);
    drawit(rxn, nameBase, true);
    delete rxn;
  }
  std::cout << " Done" << std::endl;
}

int main() {
  RDLog::InitLogs();
#if 1
  test1();
#endif
}
