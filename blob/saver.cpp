/*
 * Small utility to save images from the blob tester, using my ffmpeg build.
 * Total crap code - undocumented, unneeded, used only in testing my tester.
 *
 * Peter Jensen
 * May 2, 2017
 */


#include <iostream>
#include <string>
#include <sstream>


#ifndef INT64_C
#define UINT64_C(c) (c ## LL)
#define INT64_C(c) (c ## LL)
#endif

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

using namespace std;


namespace cs_3505
{

  void save_pictures (int * orig, int * blob, int test, int i_num, int x, int y, string tag)
  {
    // Make a filename base

    stringstream ssa;

    ssa << "test_" << test << "_image_" << i_num << "_" << tag;
    string base_name = ssa.str();

    stringstream ssb;
    ssb << base_name << "_orig.png";
    string orig_filename = ssb.str();
    
    stringstream ssc;
    ssc << base_name << "_blob.png";
    string blob_filename = ssc.str();
  
    stringstream ssd;
    ssd << base_name << "_marked.png";
    string marked_filename = ssd.str();
    
    // Initialize ffmpeg
  
    av_register_all();

    int width = 512;
    int height = 424;

    if (orig != NULL)
    {
      // Allocate a frame for the copy of the image.
  
      AVFrame *image = av_frame_alloc();
  
      if (image == NULL)
      {
	cout << "Memory allocation failed:  AVFrame in copy_frame" << endl;
	return;
      }

      // Set up destination frame information

      image->width = width;
      image->height = height;
      image->format = AV_PIX_FMT_RGB24;
  
      // Determine required buffer size for the copy of the pixel data,
      //   Allocate the buffer
  
      int neededBytes;
  
      neededBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24,
					     width,
					     height,
					     32);
  
      uint8_t *buffer = (uint8_t *) av_malloc(neededBytes * sizeof(uint8_t));

      // Set up the data buffer pointers (to point into our allocated buffer)
      //   Matches the data buffer pointers to match the desired alignment and
      //   pixel format of the image copy.

      av_image_fill_arrays(image->data, image->linesize,
			   buffer,
			   AV_PIX_FMT_RGB24, width, height, 32);


      // Put in grayscale-ish data for source intensity, 0 = purple, - = red.

      for (int tx = 0; tx < 512; tx++)
	for (int ty = 0; ty < 424; ty++)
	{
	  int offset = ((i_num * 424 + ((ty&0x7fffffff) < 0x1a8 ? ty : 0))<<9) + ((tx & (~0x1ff)) == 0 ? tx : 0);
	  int v = orig[offset];
      
	  if (v == 0)
	  {
	    image->data[0][ty * image->linesize[0] + tx*3+0] = 0x80;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = 0x00;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = 0xff;
	  }
	  else if (v < 0)
	  {
	    image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = 0x00;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = 0x00;
	  }
	  else
	  {
	    char ch = (v >> 23) & 0xf0;
	    image->data[0][ty * image->linesize[0] + tx*3+0] = ch + (v & 0x0f);
	    image->data[0][ty * image->linesize[0] + tx*3+1] = ch + ((v >> 23) & 0x0f);
	    image->data[0][ty * image->linesize[0] + tx*3+2] = ch + ((v >> 19) & 0x0f);
	  }      	
	}
  
      FILE *pngFile;

      AVCodec *pngCodec;
      AVCodecContext *pngCodecCtx; 

      // Attempts to find the PNG encoder, returns an error if the Codec cannot be found

      if ((pngCodec = avcodec_find_encoder(AV_CODEC_ID_PNG)) == NULL)
      {
	cout << "PNG codec not found" << endl;
	return;
      }

  
      // Allocates the png context, returns an error if cannot be done

      if((pngCodecCtx = avcodec_alloc_context3(pngCodec)) == NULL)
      {
	cout << "Could not allocate png context" << endl;
	return;
      }	

      //Initializes the png frame based off of the image Codec context

      pngCodecCtx->width = width;
      pngCodecCtx->height = height;
      pngCodecCtx->pix_fmt = AV_PIX_FMT_RGB24;
      pngCodecCtx->time_base.num = 1;
      pngCodecCtx->time_base.den = 1;

      //Attempts to open the codec after initializing pngCodec context info

      if(avcodec_open2(pngCodecCtx,pngCodec,NULL) < 0)
      {
	cout << "Codec open failed" << endl;
	return;
      }

  
      //frame packet code also comes from //github.com/jwerle/video-transcode-experiment/blob/master/video-transcode.c
  
      AVPacket *ppacket = av_packet_alloc();
      av_init_packet(ppacket);
      ppacket->size = 0;
      ppacket->data = NULL;

      //Encodes the video

      if (avcodec_send_frame(pngCodecCtx, image) != 0)
      {
	cout << "Could not send frame to png codec for encoding" << endl;
	return;
      }
  
      if (avcodec_receive_packet(pngCodecCtx, ppacket) != 0)
      {
	cout << "Could not get encoded png packet" << endl;
	return;
      }


      int size = ppacket->size;

      //If somehow a negative number is the buffered size, return an error due to failure.
      if(size <= 0)
      {
	cout << "Couldn't encode the video - illegal size" << endl;
	return;
      }

      //png codec opens

      pngFile = fopen (orig_filename.c_str(), "wb" ); 

      //Checks to make sure the png file is non null

      if (pngFile == NULL) 
      {
	return;
      }

      //Checks to make sure that the packet data can be written
    
      fwrite(ppacket->data, 1, ppacket->size, pngFile);
      fclose(pngFile);

      av_packet_free(&ppacket);


      //close out the file and free the AVCodecs and AVCodecContext
    
      avcodec_close(pngCodecCtx);

      av_free(pngCodecCtx);
    
      // The pixel buffer pointer still exists, it is the first (and only)
      //   plane of RGB24 data.  This is bad form.  We should hold this
      //   pointer someplace safe, just in case ffmpeg does not copy it
      //   to data[0].  (Perhaps in side data in the frame?)
 
      av_freep(&(image->data[0])); 

      // Unalloc the frame.
  
      av_frame_free(&image);
    }

    // End - original depth image output

    if (blob != NULL)
    {
      // Allocate a frame for the copy of the image.
  
      AVFrame *image = av_frame_alloc();
  
      if (image == NULL)
      {
	cout << "Memory allocation failed:  AVFrame in copy_frame" << endl;
	return;
      }

      // Set up destination frame information

      image->width = width;
      image->height = height;
      image->format = AV_PIX_FMT_RGB24;
  
      // Determine required buffer size for the copy of the pixel data,
      //   Allocate the buffer
  
      int neededBytes;
  
      neededBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24,
					     width,
					     height,
					     32);
  
      uint8_t *buffer = (uint8_t *) av_malloc(neededBytes * sizeof(uint8_t));

      // Set up the data buffer pointers (to point into our allocated buffer)
      //   Matches the data buffer pointers to match the desired alignment and
      //   pixel format of the image copy.

      av_image_fill_arrays(image->data, image->linesize,
			   buffer,
			   AV_PIX_FMT_RGB24, width, height, 32);


      // Put in colored data for blob ID, 0 = purple, + = while.

      for (int tx = 0; tx < 512; tx++)
	for (int ty = 0; ty < 424; ty++)
	{
	  int offset = ((i_num * 424 + ((ty&0x7fffffff) < 0x1a8 ? ty : 0))<<9) + ((tx & (~0x1ff)) == 0 ? tx : 0);
	  int v = blob[offset];
      	  
      
	  if (v == 0)
	  {
	    image->data[0][ty * image->linesize[0] + tx*3+0] = 0x00;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = 0x00;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = 0x00;
	  }
	  else if (v > 0)
	  {
	    image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = 0xff;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = 0xff;
	  }
	  else
	  {
	    int r = v;  // Randomize the bits a bit to distinguish blobs
	    for (int i = 0; i < 100; i++)
	      r = (r << 10) | (((r >> 18) ^ (r >> 21)) & 0x3ff);
	    
	    image->data[0][ty * image->linesize[0] + tx*3+0] = (r >> 0) & 0xff | 0x20;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = (r >> 8) & 0xff | 0x20;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = (r >> 16) & 0xff | 0x20;
	  }      	
	}
  
      FILE *pngFile;

      AVCodec *pngCodec;
      AVCodecContext *pngCodecCtx; 

      // Attempts to find the PNG encoder, returns an error if the Codec cannot be found

      if ((pngCodec = avcodec_find_encoder(AV_CODEC_ID_PNG)) == NULL)
      {
	cout << "PNG codec not found" << endl;
	return;
      }

  
      // Allocates the png context, returns an error if cannot be done

      if((pngCodecCtx = avcodec_alloc_context3(pngCodec)) == NULL)
      {
	cout << "Could not allocate png context" << endl;
	return;
      }	

      //Initializes the png frame based off of the image Codec context

      pngCodecCtx->width = width;
      pngCodecCtx->height = height;
      pngCodecCtx->pix_fmt = AV_PIX_FMT_RGB24;
      pngCodecCtx->time_base.num = 1;
      pngCodecCtx->time_base.den = 1;

      //Attempts to open the codec after initializing pngCodec context info

      if(avcodec_open2(pngCodecCtx,pngCodec,NULL) < 0)
      {
	cout << "Codec open failed" << endl;
	return;
      }

  
      //frame packet code also comes from //github.com/jwerle/video-transcode-experiment/blob/master/video-transcode.c
  
      AVPacket *ppacket = av_packet_alloc();
      av_init_packet(ppacket);
      ppacket->size = 0;
      ppacket->data = NULL;

      //Encodes the video

      if (avcodec_send_frame(pngCodecCtx, image) != 0)
      {
	cout << "Could not send frame to png codec for encoding" << endl;
	return;
      }
  
      if (avcodec_receive_packet(pngCodecCtx, ppacket) != 0)
      {
	cout << "Could not get encoded png packet" << endl;
	return;
      }


      int size = ppacket->size;

      //If somehow a negative number is the buffered size, return an error due to failure.
      if(size <= 0)
      {
	cout << "Couldn't encode the video - illegal size" << endl;
	return;
      }

      //png codec opens

      pngFile = fopen (blob_filename.c_str(), "wb" ); 

      //Checks to make sure the png file is non null

      if (pngFile == NULL) 
      {
	return;
      }

      //Checks to make sure that the packet data can be written
    
      fwrite(ppacket->data, 1, ppacket->size, pngFile);
      fclose(pngFile);

      av_packet_free(&ppacket);


      //close out the file and free the AVCodecs and AVCodecContext
    
      avcodec_close(pngCodecCtx);

      av_free(pngCodecCtx);
    
      // The pixel buffer pointer still exists, it is the first (and only)
      //   plane of RGB24 data.  This is bad form.  We should hold this
      //   pointer someplace safe, just in case ffmpeg does not copy it
      //   to data[0].  (Perhaps in side data in the frame?)
 
      av_freep(&(image->data[0])); 

      // Unalloc the frame.
  
      av_frame_free(&image);
    }

    // End - blob depth image output

    
    if (blob != NULL && x >= 0 && x < 512 && y >= 0 && y < 424)
    {
      // Allocate a frame for the copy of the image.
  
      AVFrame *image = av_frame_alloc();
  
      if (image == NULL)
      {
	cout << "Memory allocation failed:  AVFrame in copy_frame" << endl;
	return;
      }

      // Set up destination frame information

      image->width = width;
      image->height = height;
      image->format = AV_PIX_FMT_RGB24;
  
      // Determine required buffer size for the copy of the pixel data,
      //   Allocate the buffer
  
      int neededBytes;
  
      neededBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24,
					     width,
					     height,
					     32);
  
      uint8_t *buffer = (uint8_t *) av_malloc(neededBytes * sizeof(uint8_t));

      // Set up the data buffer pointers (to point into our allocated buffer)
      //   Matches the data buffer pointers to match the desired alignment and
      //   pixel format of the image copy.

      av_image_fill_arrays(image->data, image->linesize,
			   buffer,
			   AV_PIX_FMT_RGB24, width, height, 32);


      // Put in colored data for blob ID, 0 = purple, + = while.

      for (int tx = 0; tx < 512; tx++)
	for (int ty = 0; ty < 424; ty++)
	{
	  int offset = ((i_num * 424 + ((ty&0x7fffffff) < 0x1a8 ? ty : 0))<<9) + ((tx & (~0x1ff)) == 0 ? tx : 0);
	  int v = blob[offset];
      	  
      
	  if (v == 0)
	  {
	    image->data[0][ty * image->linesize[0] + tx*3+0] = 0x00;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = 0x00;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = 0x00;
	  }
	  else if (v > 0)
	  {
	    image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = 0xff;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = 0xff;
	  }
	  else
	  {
	    int r = v;  // Randomize the bits a bit to distinguish blobs
	    for (int i = 0; i < 100; i++)
	      r = (r << 10) | (((r >> 18) ^ (r >> 21)) & 0x3ff);
	    
	    image->data[0][ty * image->linesize[0] + tx*3+0] = (r >> 0) & 0xff | 0x20;
	    image->data[0][ty * image->linesize[0] + tx*3+1] = (r >> 8) & 0xff | 0x20;
	    image->data[0][ty * image->linesize[0] + tx*3+2] = (r >> 16) & 0xff | 0x20;
	  }      	
	}

      int w = 10;  
      int h = 10;
	  
      for (int dy = -10; dy <= 10; dy++)
      {
	int ty, tx;
	int dx = (int)(sqrt(1.0-pow(dy/(double)10,2))*10);

	ty = y + dy;
	tx = x + dx;
	if (ty >= 0 && ty < 424 && tx >= 0 && tx < 512)
	{
	  image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	  image->data[0][ty * image->linesize[0] + tx*3+1] = 0;
	  image->data[0][ty * image->linesize[0] + tx*3+2] = 0;
	}
	ty = y + dx;
	tx = x + dy;
	if (ty >= 0 && ty < 424 && tx >= 0 && tx < 512)
	{
	  image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	  image->data[0][ty * image->linesize[0] + tx*3+1] = 0;
	  image->data[0][ty * image->linesize[0] + tx*3+2] = 0;
	}
	ty = y + dy;
	tx = x - dx;
	if (ty >= 0 && ty < 424 && tx >= 0 && tx < 512)
	{
	  image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	  image->data[0][ty * image->linesize[0] + tx*3+1] = 0;
	  image->data[0][ty * image->linesize[0] + tx*3+2] = 0;
	}
	ty = y - dx;
	tx = x + dy;
	if (ty >= 0 && ty < 424 && tx >= 0 && tx < 512)
	{
	  image->data[0][ty * image->linesize[0] + tx*3+0] = 0xff;
	  image->data[0][ty * image->linesize[0] + tx*3+1] = 0;
	  image->data[0][ty * image->linesize[0] + tx*3+2] = 0;
	}
	 
	 
      }
  
      FILE *pngFile;

      AVCodec *pngCodec;
      AVCodecContext *pngCodecCtx; 

      // Attempts to find the PNG encoder, returns an error if the Codec cannot be found

      if ((pngCodec = avcodec_find_encoder(AV_CODEC_ID_PNG)) == NULL)
      {
	cout << "PNG codec not found" << endl;
	return;
      }

  
      // Allocates the png context, returns an error if cannot be done

      if((pngCodecCtx = avcodec_alloc_context3(pngCodec)) == NULL)
      {
	cout << "Could not allocate png context" << endl;
	return;
      }	

      //Initializes the png frame based off of the image Codec context

      pngCodecCtx->width = width;
      pngCodecCtx->height = height;
      pngCodecCtx->pix_fmt = AV_PIX_FMT_RGB24;
      pngCodecCtx->time_base.num = 1;
      pngCodecCtx->time_base.den = 1;

      //Attempts to open the codec after initializing pngCodec context info

      if(avcodec_open2(pngCodecCtx,pngCodec,NULL) < 0)
      {
	cout << "Codec open failed" << endl;
	return;
      }

  
      //frame packet code also comes from //github.com/jwerle/video-transcode-experiment/blob/master/video-transcode.c
  
      AVPacket *ppacket = av_packet_alloc();
      av_init_packet(ppacket);
      ppacket->size = 0;
      ppacket->data = NULL;

      //Encodes the video

      if (avcodec_send_frame(pngCodecCtx, image) != 0)
      {
	cout << "Could not send frame to png codec for encoding" << endl;
	return;
      }
  
      if (avcodec_receive_packet(pngCodecCtx, ppacket) != 0)
      {
	cout << "Could not get encoded png packet" << endl;
	return;
      }


      int size = ppacket->size;

      //If somehow a negative number is the buffered size, return an error due to failure.
      if(size <= 0)
      {
	cout << "Couldn't encode the video - illegal size" << endl;
	return;
      }

      //png codec opens

      pngFile = fopen (marked_filename.c_str(), "wb" ); 

      //Checks to make sure the png file is non null

      if (pngFile == NULL) 
      {
	return;
      }

      //Checks to make sure that the packet data can be written
    
      fwrite(ppacket->data, 1, ppacket->size, pngFile);
      fclose(pngFile);

      av_packet_free(&ppacket);


      //close out the file and free the AVCodecs and AVCodecContext
    
      avcodec_close(pngCodecCtx);

      av_free(pngCodecCtx);
    
      // The pixel buffer pointer still exists, it is the first (and only)
      //   plane of RGB24 data.  This is bad form.  We should hold this
      //   pointer someplace safe, just in case ffmpeg does not copy it
      //   to data[0].  (Perhaps in side data in the frame?)
 
      av_freep(&(image->data[0])); 

      // Unalloc the frame.
  
      av_frame_free(&image);
    }

    // End - marked image output

    
  }
    
  
}
