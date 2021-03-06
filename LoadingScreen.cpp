#include "LoadingScreen.h"
#include "Configuration.h"
#include "Constants.h"
#include "GenericHelperMethods.h"
#include "Logfile.h"



LoadingScreen::LoadingScreen( IrrlichtDevice* device, io::path imageFileName )
:  device_(device),
  imageFileName_(imageFileName)
{
    if ( device_ == 0 )
        Logfile::getInstance().emergencyExit( "Entchen in [LoadingScreen] nicht mehr gefunden! Abbruch." );
    createLoadingScreenImage();
    createLoadingScreenText();
    loadingScreenImageFrame_->getImage()->grab();  // don't let anyone delete your precious image!
}



LoadingScreen::~LoadingScreen()
{
    loadingScreenImageFrame_->getImage()->drop();  // you grab it, you drop it.
    loadingScreenImageFrame_->remove();
    loadingText_->remove();
}



void LoadingScreen::enable()
{
    loadingScreenImageFrame_->setEnabled( true );
    loadingScreenImageFrame_->setVisible( true );
}



void LoadingScreen::disable()
{
    loadingScreenImageFrame_->setVisible( false );
    loadingScreenImageFrame_->setEnabled( false );
}



void LoadingScreen::setText( const wchar_t* text )
{
    loadingText_->setText( text );
}



/* private */



void LoadingScreen::createLoadingScreenImage()
{
    GenericHelperMethods::getInstance().validateFileExistence( imageFileName_ );
    video::ITexture* loadingScreenImage = device_->getVideoDriver()->getTexture( imageFileName_ );
    core::recti frame = screenDependentSizeOf( *loadingScreenImage );
    loadingScreenImageFrame_ = device_->getGUIEnvironment()->addImage( frame );
    loadingScreenImageFrame_->setImage( loadingScreenImage );
    loadingScreenImageFrame_->setScaleImage( true );
    loadingScreenImageFrame_->setEnabled( false );
    loadingScreenImageFrame_->setVisible( false );
}



void LoadingScreen::createLoadingScreenText()
{
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    GenericHelperMethods& helpers = GenericHelperMethods::getInstance( device_ );
    loadingText_ = device_->getGUIEnvironment()->addStaticText(
            L"", core::recti( core::dimension2di( 9, screenSize.Height - 30 ), screenSize ) );
    loadingText_->setOverrideColor( video::SColor( 255, 128, 64, 64) );
    helpers.validateFileExistence( "GFX/FONTS/Dooling_font.xml" );
    helpers.validateFileExistence( "GFX/FONTS/Dooling_font.png" );
    helpers.validateFileExistence( "GFX/FONTS/Dooling_font_readme.txt" );
    gui::IGUIFont* font = device_->getGUIEnvironment()->getFont( "GFX/FONTS/Dooling_font.xml" );
    loadingText_->setOverrideFont( font );
}



core::recti LoadingScreen::screenDependentSizeOf( video::ITexture& image )
{
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    f32 screenWidth = static_cast<f32>( screenSize.Width );
    f32 screenHeight = static_cast<f32>( screenSize.Height );
    f32 screenRatio = screenWidth / screenHeight;
    f32 imageWidth = static_cast<f32>( image.getSize().Width );
    f32 imageHeight = static_cast<f32>( image.getSize().Height );
    f32 imageRatio = imageWidth / imageHeight;
    core::recti frame;
    if ( imageRatio > screenRatio )
    {
        f32 resizedImageHeight = ( screenWidth / imageWidth ) * imageHeight;
        f32 minHightOffset = ( screenHeight - resizedImageHeight ) / 2.0f;
        frame = core::recti(
                0,
                static_cast<u32>( minHightOffset ),
                screenSize.Width,
                static_cast<u32>( minHightOffset + resizedImageHeight )
        );
    }
    else if ( imageRatio < screenRatio )
    {
        f32 resizedImageWidth = ( screenHeight / imageHeight ) * imageWidth;
        f32 minWidthOffset = ( screenWidth - resizedImageWidth ) / 2.0f;
        frame = core::recti(
                static_cast<u32>( minWidthOffset ),
                0,
                static_cast<u32>( minWidthOffset + resizedImageWidth ),
                screenSize.Height
        );
    }
    else
    {
        frame = core::recti( VEC_2DI_NULL, screenSize );
    }
    return frame;
}
