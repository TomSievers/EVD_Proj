#include <include/VsyncCairoDrawer.hpp>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace ImageDrawer
{
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
    GPUBuffer::GPUBuffer() : gpufd(-1), width(0), height(0), stride(0), handle(0), map(NULL), fb(0), cairoContext(NULL), cairoSurface(NULL)
    {

    }

    GPUBuffer::~GPUBuffer()
    {

        if(cairoContext != NULL)
        {
            cairo_destroy(cairoContext);
        }

        if(cairoSurface != NULL)
        {
            cairo_surface_finish(cairoSurface);
            cairo_surface_destroy(cairoSurface);
        }

        if(gpufd >= 0)
        {
            struct drm_mode_destroy_dumb dreq;
            munmap(map, size);

            drmModeRmFB(gpufd, fb);

            memset(&dreq, 0, sizeof(dreq));
            dreq.handle = handle;
            drmIoctl(gpufd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
        }
    }

//#endif
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
    GPUOutput::GPUOutput() : gpufd(-1), display_buf(0), ready_buf(1), draw_buf(2), conn(0), crtc(0), saved_crtc(NULL)
    {
        memset(&mode, 0, sizeof(drmModeModeInfo));
    };


    GPUOutput::~GPUOutput()
    {
        if(gpufd >= 0)
        {
            drmModeSetCrtc(gpufd, saved_crtc->crtc_id, saved_crtc->buffer_id, saved_crtc->x, saved_crtc->y, &conn, 1, &saved_crtc->mode);
            drmModeFreeCrtc(saved_crtc);
        }
    }
//#endif
    VsyncCairoDrawer::VsyncCairoDrawer(const std::string& gpu, const std::string& terminal) : terminal(terminal), curColor(0, 0, 0, 0), active(true)
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
//#ifdef DEBUG
        std::cout << "using gpu: " << gpu << std::endl;
//#endif
        openGPU(gpu);
        prepareGPU();

        if(GPUOutList.empty())
        {
            throw(std::runtime_error("unable to find connections on gpu " + gpu));
        }

        for(auto& GPU : GPUOutList)
        {
            GPU->saved_crtc = drmModeGetCrtc(gpufd, GPU->crtc);
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->display_buf];
            int ret = drmModeSetCrtc(gpufd, GPU->crtc, buf->fb, 0, 0,
                        &GPU->conn, 1, &GPU->mode);
            if (ret)
            {
                std::cerr << "cannot set CRTC for connector " << GPU->conn << " errno: " << errno << std::endl;
            }

            screenWidth = buf->width;
            screenHeight = buf->height;
        }
        //thread = std::thread(&VsyncCairoDrawer::update, this);
//#endif
    }

    VsyncCairoDrawer::~VsyncCairoDrawer()
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        active = false;
        thread.join();
        drmEventContext ev;

        /* init variables */
        memset(&ev, 0, sizeof(ev));
        ev.version = DRM_EVENT_CONTEXT_VERSION;

        for(auto& GPU : GPUOutList)
        {
            for(auto& buf : GPU->bufs)
            {
                buf.reset();
            }
            GPU.reset();
        }

        close(gpufd);
//#endif
    }

    void VsyncCairoDrawer::openGPU(const std::string& device)
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        uint64_t has_dumb;
        gpufd = open(device.c_str(), O_RDWR | O_CLOEXEC);
        if (gpufd < 0) 
        {
            throw(std::runtime_error("unable to open device " + device + " errno: " + std::to_string(errno)));
        }

        if (drmGetCap(gpufd, DRM_CAP_DUMB_BUFFER, &has_dumb) < 0 || !has_dumb) 
        {
            throw(std::runtime_error("DRM device" + device + "does not support dumb buffers "));
        }
//#endif
    }

    void VsyncCairoDrawer::prepareGPU()
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        drmModeRes *res;
        drmModeConnector *conn;
        std::shared_ptr<GPUOutput> dev;
        int ret;

        /* retrieve resources */
        res = drmModeGetResources(gpufd);
        if (res == NULL) 
        {
            throw(std::runtime_error("cannot retrieve DRM resources " + std::to_string(errno)));
        }

        /* iterate all connectors */
        for (int i = 0; i < res->count_connectors; ++i) {
            /* get information for each connector */
            conn = drmModeGetConnector(gpufd, res->connectors[i]);
            if (conn == NULL) 
            {
                std::cerr << "cannot retrieve DRM connector " << i << ":" << res->connectors[i] << " errno: " << errno << std::endl;
                continue;
            }

            /* create a device structure */
            dev = std::make_shared<GPUOutput>();
            dev->gpufd = gpufd;
            dev->conn = conn->connector_id;

            /* call helper function to prepare this connector */
            ret = setupGPUOutput(res, conn, dev);
            if (ret) 
            {
                dev.reset();
                drmModeFreeConnector(conn);
                continue;
            }

            /* free connector data and link device into global list */
            drmModeFreeConnector(conn);
            if(dev != nullptr)
            {
                GPUOutList.push_back(dev);
            }
        }

        /* free resources again */
        drmModeFreeResources(res);
//#endif
    }

    int VsyncCairoDrawer::setupGPUOutput(drmModeRes *res, drmModeConnector *conn, std::shared_ptr<GPUOutput> dev)
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        if (conn->connection != DRM_MODE_CONNECTED) 
        {
            return -ENOENT;
        }

        /* check if there is at least one valid mode */
        if (conn->count_modes == 0) 
        {
            return -EFAULT;
        }

        for(auto& buf : dev->bufs)
        {
            buf = std::make_shared<GPUBuffer>();
        }

        /* copy the mode information into our device structure and into both
        * buffers */
        memcpy(&dev->mode, &conn->modes[0], sizeof(dev->mode));
        for(auto& buf : dev->bufs)
        {
            if(buf != nullptr)
            {
                buf->width = conn->modes[0].hdisplay;
                buf->height = conn->modes[0].vdisplay;
            } else {
                std::cerr << "unable to allocate GPUBuffer data structure" << std::endl;
                return -EFAULT;
            }
        }
//#ifdef DEBUG
        std::cout << "Mode for connector " << conn->connector_id << " is " << dev->bufs[0]->width << "x" << dev->bufs[0]->height << std::endl;
//#endif
        /* find a crtc for this connector */
        int ret = findCRTC(res, conn, dev);
        if (ret) 
        {
            std::cerr << "no valid crtc for connector " << conn->connector_id << std::endl;
            return ret;
        }

        /* create framebuffer #1 for this CRTC */
        if (fillGPUBuffer(dev->bufs[0])) 
        {
            std::cerr << "cannot create framebuffer for connector " << conn->connector_id << std::endl;
            return ret;
        }

        /* create framebuffer #2 for this CRTC */
        if (fillGPUBuffer(dev->bufs[1])) 
        {
            std::cerr << "cannot create framebuffer for connector " << conn->connector_id << std::endl;
            dev->bufs[0].reset();
            return ret;
        }

        /* create framebuffer #3 for this CRTC */
        if (fillGPUBuffer(dev->bufs[2])) 
        {
            std::cerr << "cannot create framebuffer for connector " << conn->connector_id << std::endl;
            dev->bufs[0].reset();
            dev->bufs[1].reset();
            return ret;
        }
//#endif
        return 0;
    }

    int VsyncCairoDrawer::findCRTC(drmModeRes *res, drmModeConnector *conn, std::shared_ptr<GPUOutput> dev)
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        drmModeEncoder *enc;
        int32_t crtc;

        /* first try the currently conected encoder+crtc */
        if (conn->encoder_id)
            enc = drmModeGetEncoder(gpufd, conn->encoder_id);
        else
            enc = NULL;

        if (enc != NULL) {
            if (enc->crtc_id) {
                crtc = enc->crtc_id;
                for(auto& GPU : GPUOutList)
                {
                    if (GPU->crtc == crtc) 
                    {
                        crtc = -1;
                        break;
                    }
                }

                if (crtc >= 0) 
                {
                    drmModeFreeEncoder(enc);
                    dev->crtc = crtc;
                    return 0;
                }
            }

            drmModeFreeEncoder(enc);
        }

        /* If the connector is not currently bound to an encoder or if the
        * encoder+crtc is already used by another connector (actually unlikely
        * but lets be safe), iterate all other available encoders to find a
        * matching CRTC. */
        for (int i = 0; i < conn->count_encoders; ++i) {
            enc = drmModeGetEncoder(gpufd, conn->encoders[i]);
            if (!enc) 
            {
                std::cerr << "cannot retrieve encoder " << i << ":" << conn->encoders[i] << " : " << errno << std::endl;
                continue;
            }

            /* iterate all global CRTCs */
            for (int j = 0; j < res->count_crtcs; ++j) {
                /* check whether this CRTC works with the encoder */
                if (!(enc->possible_crtcs & (1 << j)))
                    continue;

                /* check that no other device already uses this CRTC */
                crtc = res->crtcs[j];
                for(auto& GPU : GPUOutList)
                {
                    if (GPU->crtc == crtc) 
                    {
                        crtc = -1;
                        break;
                    }
                }

                /* we have found a CRTC, so save it and return */
                if (crtc >= 0) {
                    drmModeFreeEncoder(enc);
                    dev->crtc = crtc;
                    return 0;
                }
            }

            drmModeFreeEncoder(enc);
        }

        std::cerr << "cannot find suitable CRTC for connector " << conn->connector_id << std::endl;
//#endif
        return -ENOENT;
    }

    int VsyncCairoDrawer::fillGPUBuffer(std::shared_ptr<GPUBuffer> buf)
    {       
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        struct drm_mode_create_dumb creq;
        struct drm_mode_destroy_dumb dreq;
        struct drm_mode_map_dumb mreq;
        int ret;

        buf->gpufd = gpufd;

        /* create dumb buffer */
        memset(&creq, 0, sizeof(creq));
        creq.width = buf->width;
        creq.height = buf->height;
        creq.bpp = 32;
        if (drmIoctl(gpufd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) 
        {
            std::cerr << "cannot create dumb buffer errno: " << errno << std::endl;
            return -errno;
        }

        buf->stride = creq.pitch;
        buf->size = creq.size;
        buf->handle = creq.handle;

        /* create framebuffer object for the dumb-buffer */
        ret = drmModeAddFB(gpufd, buf->width, buf->height, 24, 32, buf->stride, buf->handle, &buf->fb);
        if (ret) 
        {
            std::cerr << "cannot create framebuffer errno: " << errno << std::endl;
            memset(&dreq, 0, sizeof(dreq));
            dreq.handle = buf->handle;
            drmIoctl(gpufd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
            return ret;
        }

        /* prepare buffer for memory mapping */
        memset(&mreq, 0, sizeof(mreq));
        mreq.handle = buf->handle;
        ret = drmIoctl(gpufd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);
        if (ret) 
        {
            std::cerr << "cannot map dumb buffer errno: " << errno << std::endl;
            buf.reset();
            return ret;
        }

        /* perform actual memory mapping */
        buf->map = (uint8_t *)mmap(0, buf->size, PROT_READ | PROT_WRITE, MAP_SHARED,
                    gpufd, mreq.offset);
        if (buf->map == MAP_FAILED) {
            std::cerr << "cannot map dumb buffer errno: " << errno << std::endl;
            buf.reset();
            return ret;
        }

        /* clear the framebuffer to 0 */
        memset(buf->map, 0, buf->size);

        buf->cairoSurface = cairo_image_surface_create_for_data(buf->map, CAIRO_FORMAT_ARGB32, buf->width, buf->height, buf->stride);
        buf->cairoContext = cairo_create(buf->cairoSurface);
//#endif
        return 0;
        
    }

    void VsyncCairoDrawer::update()
    {
//#if defined(HAVE_LIBDRM) && defined(HAVE_CAIRO)
        drmVBlank vblank;
        memset(&vblank, 0, sizeof(drmVBlank));
        vblank.request.type = DRM_VBLANK_RELATIVE;
        vblank.request.sequence = 1;
        
        while(active)
        {
            setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));
        
            setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));

            setLineWidth(10);

            drawLine(cv::Point(0, 0), cv::Point(getScreenWidth(), getScreenHeight()));

            drawCircle(cv::Point(1000, 100), 100);

            draw();

            //std::cout << "1. " << (int)GPUOutList.at(0)->draw_buf << " " << (int)GPUOutList.at(0)->ready_buf << " " << (int)GPUOutList.at(0)->display_buf << std::endl;
                
            swapDrawReady();

            //std::cout << "2. " << (int)GPUOutList.at(0)->draw_buf << " " << (int)GPUOutList.at(0)->ready_buf << " " << (int)GPUOutList.at(0)->display_buf << std::endl;

            setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));
        
            setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));

            setLineWidth(10);

            drawLine(cv::Point(0, 0), cv::Point(getScreenWidth(), getScreenHeight()));

            drawCircle(cv::Point(1000, 100), 100);

            draw();

            //std::cout << "3. " << (int)GPUOutList.at(0)->draw_buf << " " << (int)GPUOutList.at(0)->ready_buf << " " << (int)GPUOutList.at(0)->display_buf << std::endl;
                
            //swapDrawReady();

            drmWaitVBlank(gpufd, &vblank);
            
            for(auto& GPU : GPUOutList)
            {                
                if(GPU->swapReady.load() && GPU->bufSwap.try_lock())
                {
                    drmModeSetCrtc(gpufd, GPU->crtc, GPU->bufs[GPU->ready_buf]->fb, 0, 0, &GPU->conn, 1, &GPU->mode);
                    uint8_t temp = GPU->display_buf;
                    GPU->display_buf = GPU->ready_buf;
                    GPU->ready_buf = temp;
                    GPU->swapReady.store(false);
                    GPU->bufSwap.unlock();
                }
            }   
        }
//#endif
    }

    void VsyncCairoDrawer::swapDrawReady()
    {
        for(auto& GPU : GPUOutList)
        {
            if(GPU->bufSwap.try_lock())
            {
                uint8_t temp = GPU->draw_buf;
                GPU->draw_buf = GPU->ready_buf;
                GPU->ready_buf = temp;
                GPU->swapReady.store(true);
                GPU->bufSwap.unlock();
            }
        }
    }

    uint32_t VsyncCairoDrawer::getScreenWidth()
    {
        return screenWidth;
    }

    uint32_t VsyncCairoDrawer::getScreenHeight()
    {
        return screenHeight;
    }

    void VsyncCairoDrawer::setDrawColor(const ColorRGBInt& color)
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            curColor.r = color.r;
            curColor.b = color.b;
            curColor.g = color.g;
            curColor.a = 255;
            cairo_set_source_rgb(buf->cairoContext, color.r/255.0F, color.b/255.0F, color.g/255.0F);
        }
//#endif
    }

    void VsyncCairoDrawer::setDrawColor(const ColorRGBAInt& color)
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            curColor.r = color.r;
            curColor.b = color.b;
            curColor.g = color.g;
            curColor.a = color.a;
            cairo_set_source_rgba(buf->cairoContext, color.r/255.0F, color.b/255.0F, color.g/255.0F, color.a/255.0F);
        }
//#endif
    }

    void VsyncCairoDrawer::setBackground(const ColorRGBInt& color)
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            cairo_set_source_rgb(buf->cairoContext, color.r, color.b, color.g);
            cairo_move_to(buf->cairoContext, 0, 0);
            cairo_rectangle(buf->cairoContext, 0, 0, buf->width, buf->height);
            cairo_fill(buf->cairoContext);
            cairo_stroke(buf->cairoContext);
            cairo_set_source_rgba(buf->cairoContext, curColor.r/255.0F, curColor.b/255.0F, curColor.g/255.0F, curColor.a/255.0F);
            cairo_move_to(buf->cairoContext, 0, 0);            
        }
        
//#endif
    }

    void VsyncCairoDrawer::drawCircle(const cv::Point& center, double radius)
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            cairo_move_to(buf->cairoContext, center.x+radius, center.y);
            cairo_arc(buf->cairoContext, center.x, center.y, radius, 0.0, 2.0 * M_PI);
        }
        
//#endif
    }

    void VsyncCairoDrawer::drawLine(const cv::Point& pointA, const cv::Point& pointB)
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            cairo_move_to(buf->cairoContext, pointA.x, pointA.y);
            cairo_line_to(buf->cairoContext, pointB.x, pointB.y);
        }
//#endif
    }

    void VsyncCairoDrawer::setLineWidth(int thickness)
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            cairo_set_line_width(buf->cairoContext, thickness);
        }
        
//#endif
    }

    void VsyncCairoDrawer::draw()
    {
//#if defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM)
        for(auto& GPU : GPUOutList)
        {
            std::shared_ptr<GPUBuffer> buf = GPU->bufs[GPU->draw_buf];
            cairo_stroke(buf->cairoContext);
        }
        
//#endif
    }
}
