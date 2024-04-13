#pragma once

#include <string>
#include <unordered_map>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace sm {
    namespace netHelper {
        enum class MIME {
            text_html,
            text_css,
            text_plain,
            application_javascript,
            application_json,
            application_xml,
            video_x_flv,
            video_x_matroska, 
            image_png,
            image_jpeg,
            image_gif,
            image_bmp,
            image_vnd_microsoft_icon,
            image_tiff,
            image_svg_xml
        };

        inline static const boost::bimap<MIME, std::string> MIMEType = 
            boost::assign::list_of<boost::bimap<MIME, std::string>::relation>
            ( MIME::text_html, "text/html" ) 
            ( MIME::text_css, "text/css" )
            ( MIME::text_plain, "text/plain" )
            ( MIME::application_javascript, "application/javascript" )
            ( MIME::application_json, "application/json" )
            ( MIME::application_xml, "application/xml" )
            ( MIME::video_x_flv, "video/x-flv" )
            ( MIME::video_x_matroska, "video/x-matroska" ) 
            ( MIME::image_png, "image/png" )
            ( MIME::image_jpeg, "image/png" )
            ( MIME::image_gif, "image/gif" )
            ( MIME::image_bmp, "image/bmp" )
            ( MIME::image_vnd_microsoft_icon, "image/vnd.microsoft.icon" )
            ( MIME::image_tiff, "image/tiff" )
            ( MIME::image_svg_xml, "image/svg+xml" );
        
        inline static const std::unordered_map<std::string, MIME> extensionToMIME = {
            { ".htm", MIME::text_html },
            { ".html", MIME::text_html },
            { ".php", MIME::text_html },
            { ".css", MIME::text_css },
            { ".txt", MIME::text_plain },
            { ".js", MIME::application_javascript },
            { ".json", MIME::application_json },
            { ".xml", MIME::application_xml },
            { ".flv", MIME::video_x_flv },
            { ".mkv", MIME::video_x_matroska },
            { ".png", MIME::image_png },
            { ".jpe", MIME::image_jpeg },
            { ".jpeg", MIME::image_jpeg },
            { ".jpg", MIME::image_jpeg },
            { ".gif", MIME::image_gif },
            { ".bmp", MIME::image_bmp },
            { ".ico", MIME::image_vnd_microsoft_icon },
            { ".tiff", MIME::image_tiff },
            { ".tif", MIME::image_tiff },
            { ".svg", MIME::image_svg_xml },
            { ".svgz", MIME::image_svg_xml }
        };

        inline std::string inUTF8(std::string_view contentType) {
            std::string result{ contentType };
            result += "; charset=utf-8;";
            return result;
        }
    };
}
