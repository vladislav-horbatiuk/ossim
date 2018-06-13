//---
//
// License: MIT
//
//---

// System includes:
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>

// ossim includes:  These are here just to save time/typing...
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimCsvFile.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossim2dBilinearTransform.h>
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimIndexToRgbLutFilter.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/init/ossimInit.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/Thread.h>
#include <ossim/projection/ossimNitfRpcModel.h>
#include <ossim/projection/ossimQuickbirdRpcModel.h>
#include <ossim/projection/ossimEnviCgModel.h>

// Put your includes here:

int main(int argc, char *argv[])
{
   int returnCode = 0;
   if (argc < 2)
   {
      cout<<"\nUsage: "<<argv[0]<<" <filename.geoc>\n"<<endl;
      return 1;
   }
   
   ossimArgumentParser ap(&argc, argv);
   ossimInit::instance()->addOptions(ap);
   ossimInit::instance()->initialize(ap);

   ossimFilename fname (argv[1]);

   try
   {
      ossimEnviCgModel model;
      if (model.loadEnviGeocFile(fname))
      {
         ossimFilename geomFname(fname);
         geomFname.setExtension("geom");
         model.saveCoarseGrid(geomFname);
         ossimKeywordlist kwl;
         model.saveState(kwl);
         kwl.add("type", "ossimCoarseGridModel");
         kwl.write(geomFname);
      }
   }
   catch(const ossimException& e)
   {
      ossimNotify(ossimNotifyLevel_WARN) << e.what() << std::endl;
      returnCode = 1;
   }
   catch( ... )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossim-foo caught unhandled exception!" << std::endl;
      returnCode = 1;
   }
   
   return returnCode;
}