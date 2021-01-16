#ifndef VSYNCCAIRODRAWER_HPP
#define VSYNCCAIRODRAWER_HPP

#include <include/IImageDrawer.hpp>
#include <array>
#include <string>
#include <iostream>
#include <memory>
#include <cstdint>
#include <thread>
#include <mutex>
#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
#include <cairo.h>
#include <linux/fb.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#else
struct cairo_format_t
{
    int STUB = 0;
};

struct drmModeRes
{
    int STUB = 0;
};

struct drmModeConnector
{
    int STUB = 0;
};

#endif


namespace ImageDrawer
{
    enum TTY_MODE
    {
        GRAPHICS,
        TEXT
    };

    struct GPUBuffer 
    {
        int gpufd;
        uint32_t width;
        uint32_t height;
        uint32_t stride;
        uint64_t size;
        uint32_t handle;
        uint8_t *map;
        uint32_t fb;
#ifdef HAVE_CAIRO
        cairo_t* cairoContext;
        cairo_surface_t *cairoSurface;
        GPUBuffer();
        ~GPUBuffer();
#endif
    };

    struct GPUOutput {
        std::mutex bufSwap;
        int gpufd;
        std::shared_ptr<GPUOutput> next;

        uint8_t display_buf;
        uint8_t ready_buf;
        uint8_t draw_buf;
        
        std::array<std::shared_ptr<GPUBuffer>, 3> bufs;
       
        uint32_t conn;
        int32_t crtc;
#ifdef HAVE_LIBDRM
        drmModeModeInfo mode;
        drmModeCrtc *saved_crtc;
        GPUOutput();
        ~GPUOutput();
#endif
    };

    class VsyncCairoDrawer : public IImageDrawer
    {
    public:
        /**
         * @brief Set the RGB color for every object on the canvas. 
         * 
         * @param color the color RGB of the objects on the canvas
         */
        virtual void setDrawColor(const ColorRGBInt& color);
        /**
         * @brief Set the RGBA color for every object on the canvas. 
         * 
         * @param color the color RGBA of the objects on the canvas
         */
        virtual void setDrawColor(const ColorRGBAInt& color);
        /**
         * @brief Set the Background to a color. Erases all curent objects drawn on screen
         * 
         * @param color 
         */
        virtual void setBackground(const ColorRGBInt& color);
        /**
         * @brief Draw a circle on the screen at the position with a radius
         * 
         * @param center the center of the circle
         * @param radius the radius of the circle
         */
        virtual void drawCircle(const cv::Point& center, double radius);
        /**
         * @brief Draw a line from point a to point b
         * 
         * @param pointA starting point of the line
         * @param pointB end point of the line
         */
        virtual void drawLine(const cv::Point& pointA, const cv::Point& pointB);
        /**
         * @brief Set the Line width of every object on the canvas.
         * 
         * @param thickness the thickness of the lines
         */
        virtual void setLineWidth(int thickness);
        /**
         * @brief Commit drawn shapes to be drawn on the screen
         * 
         */
        virtual void draw();
        /**
         * @brief Construct a new Cairo Drawer object
         * 
         * @param framebuffer path to the file discriptor of gpu
         * @param terminal path to the current terminal connected to the output display
         * @param format color format of the screen
         */
        VsyncCairoDrawer(const std::string& gpu, const std::string& terminal, cairo_format_t format);
        virtual ~VsyncCairoDrawer();
        uint32_t getScreenWidth();
        uint32_t getScreenHeight();
        void swapDrawReady();
    private:
        void setTty(const std::string& device, TTY_MODE mode);
        void openGPU(const std::string& device);
        void prepareGPU();
        int setupGPUOutput(drmModeRes *res, drmModeConnector *conn, std::shared_ptr<GPUOutput> dev);
        int fillGPUBuffer(std::shared_ptr<GPUBuffer> buf);
        int findCRTC(drmModeRes *res, drmModeConnector *conn, std::shared_ptr<GPUOutput> dev);
        
        void update();

        unsigned char* fbp;
        int fbfd;
        int gpufd;
        const std::string terminal;
        unsigned long screensize;
        uint32_t screenWidth;
        uint32_t screenHeight;
        std::shared_ptr<GPUOutput> GPULinkedList;
        bool active;
        std::thread thread;
        
    }; //CairoDrawer

} // namespace ImageDrawer

#endif //VSYNCCAIRODRAWER_HPP
