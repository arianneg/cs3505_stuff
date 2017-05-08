#include<iostream>
namespace cs_3505{
/******************************************************************************
   * void mark_blobs (Blob *results,  
   *                  int *data, int image_count,
   *                  int near_threshold, int far_threshold)
   *
   *   This function finds and marks blobs in image data.  The image data and
   * results arrays are modified to mark blobs.  
   *
   * results:  A pointer to an existing array of Blob structs.  The array will
   *           have exactly 'image_count' entries.  The initial contents are
   *           undefined.
   *
   * image_data: A pointer to depth map data.  The data is linearized 2D data for
   *             'image_count' depth maps.  The value for image i, row r, and
   *             column c is located at index i*(512*424) + r*(512) + c.  This
   *             array exists and will have i*r*c int values in it.
   *
   * image_count: The number of images that need to have blobs marked.
   *
   * near_threshold: The near threshold value.  Any depths less than this
   *                 threshold are changed to 0.
   *
   * far_threshold:  The far threshold value.  Any depths greater than this threshold are
   *                 changed to 0.
   *****************************************************************************/
void mark_blobs (Blob *results, int *data, int image_count, int near_threshold, int far_threshold){


  std::cout <<"Hello"<<std::endl;

  return;

}
  
}
