/////////////////////////////////////////////////////////////////////////////
// Name:        controller.h
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_CONTROLLER_H__
#define __VRV_CONTROLLER_H__

#include "doc.h"
#include "view.h"

#include <string>

namespace vrv {

typedef enum _file_formats {
    mei_file = 0,
    pae_file,
    darms_file
} ConvertFileFormat;


//----------------------------------------------------------------------------
// InterfaceController
//----------------------------------------------------------------------------

class InterfaceController
{
public:
    // constructors and destructors
    InterfaceController();
    virtual ~InterfaceController();
    
    /**
     * Load a file with the specified type.
     */
    bool LoadFile( std::string filename );
    
    /**
     * Load a string data witht he specified type.
     */
    bool LoadString( std::string data );
    
    /**
     * Parse the options passed as JSON string
     * only available for Emscripten based compiles
     **/
    bool ParseOptions( std::string json_options );
    
    /**
     * Render the page in SVG and returns it as a string
     * Page number is 1-based
     */
    std::string RenderToSvg( int pageNo =  1, bool xml_tag = false );

    /**
     * Render the page in SVG and save it to the file.
     * Page number is 1-based.
     */
    bool RenderToSvgFile( std::string filename, int pageNo =  1 );
    
    /**
     * @name Set and get the border
     */
    ///@{
    bool SetBorder( int border );
    int GetBorder() { return m_border; };
    ///@}
    
    /**
     * @name Set and get the scale
     */
    ///@{
    bool SetScale( int scale );
    int GetScale() { return m_scale; };
    ///@}
    
    /**
     * @name Set and get the page height (in pixels)
     */
    ///@{
    bool SetPageHeight( int h );
    int GetPageHeight() { return m_pageHeight; };
    ///@}
    
    /**
     * @name Set and get the page width (in pixels)
     */
    ///@{
    bool SetPageWidth( int w );
    int GetPageWidth() { return m_pageWidth; };
    ///@}
    
    /**
     * @name Ignore all encoded layout information (if any) 
     * and output one single page with one single system
     */
    ///@{
    void SetNoLayout( bool l ) { m_noLayout = l; };
    int GetNoLayout() { return m_noLayout; };
    ///@}
    
    /**
     * @name Ignore all encoded layout information (if any) 
     * and fully recalculate the layout 
     */
    ///@{
    void SetIgnoreLayout( bool l ) { m_ignoreLayout = l; };
    int GetIgnoreLayout() { return m_ignoreLayout; };
    ///@}
    
    /**
     * @name Crop the page height to the height of the content 
     */
    ///@{
    void SetAdjustPageHeight( bool a ) { m_adjustPageHeight = a; };
    int GetAdjustPageHeight() { return m_adjustPageHeight; };
    ///@}
    
    /**
     * @name Do not justify the system (for debugging purposes) 
     */
    ///@{
    void SetNoJustification( bool j ) { m_noJustification = j; };
    int GetNoJustification() { return m_noJustification; };
    ///@}
    
    /**
     * @name Do not justify the system (for debugging purposes) 
     */
    ///@{
    void SetShowBoundingBoxes( bool b ) { m_showBoundingBoxes = b; };
    int GetShowBoundingBoxes() { return m_showBoundingBoxes; };
    ///@}
    
    /**
     * @name Get the input file format (defined as ConvertFileFormat)
     * The SetFormat with ConvertFileFormat does not perform any validation
     */
    ///@{
    bool SetFormat( std::string informat );
    void SetFormat( ConvertFileFormat format ) { m_format = format; };
    int GetFormat() { return m_format; };
    ///@}
	
   /**
    * @name Get the pages for a loaded file
    * The SetFormat with ConvertFileFormat does not perform any validation
    */
   ///@{
   int GetPageCount( );
   ///@}
    
private:
    
public:
    
private:
    Doc m_doc;
    View m_view;
    int m_scale;
    ConvertFileFormat m_format;
    
    int m_pageHeight;
    int m_pageWidth;
    int m_border; // to be replace by pageRightMar, pageLeftMar, pageTopMar
    int m_unit;
    
    bool m_noLayout;
    bool m_ignoreLayout;
    bool m_adjustPageHeight;
    // for debugging
    bool m_noJustification;
    bool m_showBoundingBoxes;
};

} // namespace vrv
#endif
