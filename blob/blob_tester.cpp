/******************************************************************************
 * Test application for blob detection extra credit.  Its basic functionality
 * is to create test images, execute blob detection, and test the results for
 * validity.  All results are timed and brief results output to the console.
 *
 * The limits on the number of tests/batches are defined below, or may be
 * redefined at compilation time if you uncommend the ifndef/endif lines.
 *
 * Students are free to execute this file, but must not submit any portion of
 * it.  Do not copy anything from it, and do not assume you can change it.
 * I will use my own copy of this code for testing student solutions.
 * I will most likely change identifiers and code in this tester, so
 * you may not use any symbols or functions from this file.  Also, students
 * must not assume that passing these initial tests means their work is
 * complete.  It is each student's responsibility to identify possible failures
 * in their code, find ways to test for them, and fix them.
 *
 * P.S.  I make no apologies for my inability to spel.  Also, this is first-pass
 * code, and shows roughness around the edges.  I've tested it, but I may have
 * missed something.  Feel free to let me know and I'll try to post fixes.  (Fix
 * it on your own before then.)
 *
 * Version 0.1 - not yet complete.
 *
 * Peter Jensen
 * April 30, 2017
 *****************************************************************************/

//#ifndef TEST_MAX
#define TEST_MAX 10000
//#endif

//#ifndef BATCH_MAX
#define BATCH_MAX 500
//#endif

// Include the needed libraries.

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;

// Everything should be in our cs_3505 namespace for compatibility.

namespace cs_3505
{
  /******************************************************************************
   * struct Blob
   *
   *   A structure for reporting the results of blob detection in an image.
   * It conveys information about a blob.  For this project, for each image,
   * the largest blob in each image is identified.  The information for this
   * largest blob is put into this struct, and an array of these structs
   * is used to contain the results for a batch of images.
   *****************************************************************************/
   
  struct Blob
  {
    int    blob_area;
    double blob_center_row, blob_center_column;
    double blob_average_depth;
  };
  
  // Forward declaration - this function should be in a separate file
  //   supplied by the student.  This separate file is the student's sumbission.
  //   (Students must not put their blob detector in this file.)
  
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
  void mark_blobs (Blob *results, int *data, int image_count, int near_threshold, int far_threshold);
  

  /******************************************************************************
   * -- Undocumented external debugging function (uses ffmpeg) --
   *
   * I did configure it to save three pictures
   *   - One image represents the original depth data in a grayscale mode.
   *     0 is purple, and larger values are brighter.
   *   - The next two images show how the blobs were marked.  Each color
   *     maps to a single blob identifier.  Different colors represent different
   *     blob marks (-1, -2, etc.)
   *   - The final image (if relevant) draws a circle around a pixel that was
   *     the error source.
   *****************************************************************************/
  void save_pictures (int*, int *, int, int, int, int, string);
  
  
  /******************************************************************************
   * struct Parameters
   *
   * A helper struct for parsing and encapsulating command- ine parameters.
   * The constructor parses the command line arguments, and sets the error flag
   * if the command line arguments are invalid.  Otherwise, the settings for
   * the current run of this application are directly accessible in this struct.
   *
   * Note:  The random number generator seed is set as a side effect of parsing.
   *****************************************************************************/

  struct Parameters
  {
    long int seed;
    long int test_count;
    long int batch_size;
    bool     verbose;
    bool     error;

    /* Constructor
     *
     * Pass in the command line arguments, they'll be parsed and the
     * parameters set.  As a side effect, the random seed is set to
     * guarantee repeatability.
     */  
    Parameters (int argc, char** argv)
      {
        // Defaults

        seed = 1;
	test_count = 1;
	batch_size = 1;
	verbose = false;
        error = false;
	
	srand(seed);

	// Avoid duplicate arguments.

	bool seen_seed = false;
	bool seen_test_count = false;
	bool seen_batch_size = false;

	// Parse any numbers in the arguments.  
	
	long int parsed_value[argc+1];

	for (int i = 1; i < argc; i++)
	  parsed_value[i] = strtol(argv[i], NULL, 10);
	
	parsed_value[argc] = 0;  // Needed if last argument needs a number, but none is supplied.

	// Parse the arguments.
	
	for (int i = 1; i < argc; i++)  // i gets advanced below, some parameters have two parts (name/number)
	{
	  if (!strcmp(argv[i], "--seed") || !strcmp(argv[i], "-s"))
	  {
	    i++;
	    
	    if (seen_seed)
	    {
	      cout << "Aborted - Duplicate seed argument." << endl;
	      error = true;
	      return;
	    }
	    else if (parsed_value[i] <= 0 || parsed_value[i] > RAND_MAX)
	    {
	      error = true;
	      cout << "Aborted - Illegal seed value: " << parsed_value[i] << endl;
	      cout << "Valid seeds are between 1 and" << RAND_MAX << "." << endl;
	      return;
	    }
	    else
	    {
	      
	      seed = parsed_value[i];
	      srand(seed);
	      seen_seed = true;
	      continue;
	    }
	  }
      
	  if (!strcmp(argv[i], "--tests") || !strcmp(argv[i], "-t"))
	  {
	    i++;
	      
	    if (seen_test_count)
	    {
	      cout << "Aborted - Duplicate test repetition count argument." << endl;
	      error = true;
	      return;
	    }
	    else if (parsed_value[i] <= 0 || parsed_value[i] > TEST_MAX)
	    {
	      error = true;
	      cout << "Aborted - Illegal test repetition count: " << parsed_value[i] << endl;
	      cout << "You can run between 1 and " << TEST_MAX << " tests." << endl;
	      return;
	    }
	    else
	    {
	      test_count = parsed_value[i];
	      seen_test_count = true;
	      continue;
	    }
	        
	  }	
	  
	  if (!strcmp(argv[i], "--batch") || !strcmp(argv[i], "-b"))
	  {
	    i++;
	    
	    if (seen_batch_size)  
	    {
	      cout << "Aborted - Duplicate batch size argument." << endl;
	      error = true;
	      return;
	    }
	    else if (parsed_value[i] <= 0 || parsed_value[i] > BATCH_MAX)
	    {
	      error = true;
	      cout << "Aborted - Illegal image batch size: " << parsed_value[i] << endl;
	      cout << "You can test between 1 and " << BATCH_MAX << " images in each batch." << endl;
	      return;
	    }
	    else
	    {
	      batch_size = parsed_value[i];
	      seen_batch_size = true;
	      continue;
	    }
	  }
	
	  if (!strcmp(argv[i], "--verbose") || !strcmp(argv[i], "-v"))
	  {
	    verbose = true;
	    continue;
	  }
      
	  if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
	  {
	    cout << "Aborted - Help information:" << endl;
	    cout << "Blob detection test utility for CS 3505" << endl;
	    cout << "Usage:" << endl;
	    cout << "  " << argv[0] << " (optional arguments from below)" << endl;
	    cout << "Optional arguments:" << endl;
	    cout << "  --seed n    Random seed, allows repeating of experiments" << endl;
	    cout << "              Default is 1, valid range is [1.." << RAND_MAX << "]" << endl;
	    cout << "  --tests n   Number of tests (calls to blob detector) to make" << endl;
	    cout << "              Default is 1, valid range is [1.." << TEST_MAX << "]" << endl;
	    cout << "  --batch n   Number of depth images in each test" << endl;
	    cout << "              Default is 1, valid range is [1.." << BATCH_MAX << "]" << endl;
	    cout << "  --verbose   Turn on debugging output" << endl;
	    cout << "  --help      This help page, supresses application activity" << endl;
	    cout << "Each argument can be abbreviated as a single dash and first letter." << endl;
	    error = true;
	    return;
	  }	 

	  // All parameters above either return or continue the loop.
	  
	  cout << "Aborted - Unknown parameter:  " << argv[i] << endl;
	  cout << "For help:  " << argv[0] << " --help" << endl;
	  error = true;
	  return;
	}
    
      }
  };

  /* End Parameters struct */
  
  /******************************************************************************
   * class DepthTest
   *
   * A class for encapsulating depth data and validation.  The constructor 
   * creates random depth maps.  The class exposes the test parameters as
   * public fields.  The user executes their tests, the calls the validate
   * function to check the blob detection for accuracy.
   *
   * This code is NOT intented to be instructional code.  I have obscured many
   * elements in this code to prevent ideas from leaking.  Students should be
   * developing their own blob detection code, and developing their own tests.
   * Do not try to reverse engineer this code, or you may waste a lot of time.
   *****************************************************************************/
  class DepthTest
  {
  private:
    int  *const orig_blob_data;

    void set_pixel (int *const&d, int i, int x, int y, int v);
    int  get_pixel (int *const&s, int i, int x, int y);
      
    const bool  verbose;    // Copied out because this is constant for this test.
    const int   batch_size; // Copied out because this is constant for this test.
    const int   test_number;  // For error reporting only
    
  public:

    DepthTest(Parameters parms, int);
    ~DepthTest();
    
    bool validate ();

    // These fields may be directly used.

    Blob *const results;
    int  *const blob_data;
    int   image_count;
    int   near_threshold;
    int   far_threshold;
  };
  
  /******************************************************************************
   * Constructor - DepthTest::DepthTest (Parameters parms)
   *
   *   Creates random test data for a blob detection test.
   *****************************************************************************/
  DepthTest::DepthTest (Parameters parms, int test_number)
    : results        (new Blob[parms.batch_size]),
      orig_blob_data (new int[512*424*parms.batch_size]),
      blob_data      (new int[512*424*parms.batch_size]),
      verbose        (parms.verbose),
      batch_size     (parms.batch_size),
      test_number    (test_number)
  {
    image_count = parms.batch_size;
    near_threshold = 0;
    far_threshold = 0;
      
    // Make a nice range of thresholds using the entire integer range, weighted to the quarter points.
      
    while (near_threshold <= 0 ||
	   near_threshold >= far_threshold)
    {
      near_threshold = 0x40000000 + (rand() % 3 - 1) * (int)pow(rand() & 0x3FF,3);
      far_threshold  = 0x40000000 + (rand() % 3 - 1) * (int)pow(rand() & 0x3FF,3);
    }

    if (verbose)
    {
      cout << "  Near threshold:  " << near_threshold << endl;
      cout << "  Far threshold:  "  << far_threshold  << endl;
      cout << "  Generating random depth data (long operation)..." << flush;
    }

    // Randomly draw a bunch of circles to the image with slightly noisy pixels of
    //   random colors.  (This is a slow operation.)

    for (int i = 0; i < batch_size; i++)
    {
      // Wipe the background to 0.

      for (int x = 0; x < 512; x++)
	for (int y = 0; y < 424; y++)
	  set_pixel(blob_data, i, x, y, 0);

      // Draw (fill in) a bunch of random ovals of random intensity.

      int count = rand() % 1000 + 1;
      if (count == 1000)
	count = 10000;

      for (int c = 0; c < count; c++)
      {
	int cx = rand()%512;
	int cy = rand()%424;
	int w = (int) (256 * pow(rand()/(double)RAND_MAX,2));  // Hack - favor smaller 
	int h = (int) (212 * pow(rand()/(double)RAND_MAX,2));
	int v = rand();
	  
	for (int y = cy-h; y <= cy+h; y++)
	{
	  int dx = (int)(sqrt(1.0-pow((y-cy)/(double)h,2))*w);
	  for (int x = cx - dx; x <= cx + dx; x++)
	    set_pixel(blob_data, i, x, y, v);
	}
      }
	
      // Make sure the perimeter is 0.

      for (int x = 0; x < 512; x++)
      {
	set_pixel(blob_data, i, x, 0, 0);
	set_pixel(blob_data, i, x, 423, 0);
      }

      for (int y = 0; y < 424; y++)
      {
	set_pixel(blob_data, i, 0, y, 0);
	set_pixel(blob_data, i, 511, y, 0);
      }

      if (verbose)
	cout << "." << flush;
    }
      
    // Copy the original depth data so the blob detection code can work with a separate copy.
      
    memcpy(orig_blob_data, blob_data, sizeof(int)*512*424*parms.batch_size);

    if (verbose)
      cout << endl;    
  }
  
  /******************************************************************************
   * Destructor - DepthTest::~DepthTest ()
   *
   *   Frees allocated memory.  Caution - the pointers are const.  Once the
   * destructor is called, the object must not be used.  I recommend
   * only creating DepthTest objects on the stack to guarantee this condition.
   *****************************************************************************/
  DepthTest::~DepthTest ()
  { 
    delete [] results;
    delete [] orig_blob_data;
    delete [] blob_data;
  }
  
  /******************************************************************************
   * bool DepthTest::validate ()
   *
   *   Checks that the blob detection correctly populated the blob data with
   * blob identifiers (negative numbers), correctly removed values on either
   * side of the thresholds, and correctly reported largest blob information.
   *****************************************************************************/
  bool DepthTest::validate ()
  {
    if (verbose)
      cout << "  Validating results (long operation)..." << flush;

    // Validation code intentionally obscure to avoid copying.
    
    // Loop through images in the batch.

    for (int i = 0; i < batch_size; i++)
    {
      // Make sure thresholds are done right.

      unsigned int m = far_threshold - near_threshold;
      unsigned int d = near_threshold;
      int bn = 0;

      int r[512][424];
      int s[512][424];
      
      for (int tx = 0; tx < 512; tx++)
	for (int ty = 0; ty < 424; ty++)
	{
	  int t = r[tx][ty] = get_pixel(blob_data, i, tx, ty);
	  
	  bool out = ((unsigned int)(get_pixel(orig_blob_data, i, tx, ty)-d)) > m || !get_pixel(orig_blob_data, i, tx, ty);

	  if (out && t || !out && t >= 0)
	  {
	    if (verbose)
	    {
	      save_pictures (orig_blob_data, blob_data, test_number, i, tx, ty, "err");
	      cout << endl << "  ERROR:  Thresholds not observed, or blob not marked." << endl;
	      cout << near_threshold << endl;
	      cout << far_threshold << endl;
	      cout << get_pixel(orig_blob_data, i, tx, ty) << endl;
	      cout << get_pixel(blob_data, i, tx, ty) << endl;
	      cout << tx << " " << ty << endl;
	    }
	    return false;
	  }
	  
	  bn = max(bn, -t);
	}

      // Check blob connectivity.

      vector<int> bx(bn+1), by(bn+1), bc(bn+1);
      vector<double> bd(bn+1);
      int px = 0, py = 0, pv = 0, ps = 0, rv;

      // I want students to collect their own data, so I've made this code
      //   essentially useless for debugging your errors.  It finds them,
      //   but it doesn't tell you why.  I do provide images of the data, though.
      //   Still, you'll have to collect your own data for fixing your errors.
      //   (This is part of the work - figuring out how to find your errors.)

      while (!(r[px][py] ^= (rv = r[px][py]))) // A nasty, evil bunch of code for fun and to keep students from copying / reverse engineering it.  It was a ton of fun to write, but awful to debug.
      {
        if (!rv && !ps)
	{	  
	  if (!(pv=0) && ++px >= 512 && !(px = 0) && ++py >= 424)
	    break; continue;
	}
	else if (!rv && ps%5==0)
	{
	  switch(ps/10)
	  {
	  case 1: py++; break;
	  case 2: px--; break;
	  case 3: py--; break;
	  case 4: px++; break;
	  }
	  ps = s[px][py]; 	    	    	  
	}
	else
	{
	  if (bc[-rv]++ && !pv || pv && pv != rv)
	  {
	    if (verbose)
	    {
	      save_pictures (orig_blob_data, blob_data, test_number, i, px, py, "err");
	      cout << endl << "  ERROR:  Blobs incorrectly marked." << endl;
	    }
	    return false;
	  }
	  pv = rv;
	  bx[-rv] += px;
	  by[-rv] += py;
	  bd[-rv] += get_pixel(orig_blob_data, i, px, py); 
	  //set_pixel(blob_data, i, px, py, 1);  // Used in debugging the tester.
	}
	s[px][py] = ++ps;
	switch(ps%10)
	{
	case 1: py--; break;
	case 2: px++; break;
	case 3: py++; break;
	case 4: px--; break;
	case 5: ps = ps/10;
	}
  	ps = ps%10*10;
      }
      
      // Verify every blob pixel was visited and tested for accuracy.

      for (int tx = 0; tx < 512; tx++)
	for (int ty = 0; ty < 424; ty++)
	{
	  int t = get_pixel(blob_data, i, tx, ty);
	  if (t && (!s[tx][ty] || r[tx][ty]))
	  {
	    if (verbose)
	      save_pictures (orig_blob_data, blob_data, test_number, i, tx, ty, "miss");
	    
	    cout << endl << "  WARNING: Tester did not fully test an image.  Test is invalid." << endl;
	    return false;
	  }
	}

      // Verify blob data.  Since I didn't give a tiebreak, I'll accept either answer in the case of
      //   a tie, but the answer must match valid data for one of the blobs that is the largest.
      
      int ba = 0;
      for (int b = 1; b <= bn; b++)
      {
	if (bc[b] == 0)
	{
	  if (verbose)
	  {
	    save_pictures (orig_blob_data, blob_data, test_number, i, -1, -1, "err");
	    cout << endl << "  ERROR:  Non-sequential blob numbering." << endl;
	  }
	  return false;
	}
	ba = max (ba, bc[b]);
      }

      bool ok = ba == 0 && (results[i].blob_average_depth == 0 ||
			    results[i].blob_center_row == 0 ||
			    results[i].blob_center_column == 0 ||
			    results[i].blob_area == 0);
	  
      for (int b = 1; b <= bn && !ok; b++)
      {
	if (bc[b] == ba && results[i].blob_area == ba)
        {
	  if (abs(results[i].blob_average_depth - bd[b] / (double) bc[b]) < 0.5 &&
	      abs(results[i].blob_center_row - by[b] / (double) bc[b]) < 1e-8 &&
	      abs(results[i].blob_center_column - bx[b] / (double) bc[b]) < 1e-8)
	    ok = true;
        }
      }
      
      if (!ok)
      {
	save_pictures (orig_blob_data, blob_data, test_number, i, -1, -1, "err");
	cout << endl << "  ERROR:  Blob struct summary data not accurate." << endl;
	return false;
      }

      if (verbose)
	cout << "." << flush;

      // Just in case you want memories...
      
      //save_pictures (orig_blob_data, blob_data, test_number, i, -1, -1, "ok");
    }	
    
    if (verbose)
      cout << endl;

    return true;
  }
  
  /******************************************************************************
   * int DepthTest::set_pixel (int *const&d, int i, int x, int y, int v)
   *
   *   Description omitted.  Students might be tempted to copy this
   * function.  Don't.  It doesn't work entirely right for your purposes.
   *****************************************************************************/
  void DepthTest::set_pixel (int *const&d, int i, int x, int y, int v)
  {
    int offset = ((i % image_count * 424 + ((y&0x7fffffff) < 0x1a8 ? y : 0))<<9) + ((x & (~0x1ff)) == 0 ? x : 0);
    
    // Obscure code to avoid copying.

    d[offset] = (v ? v+rand()%10000001-5000000 : v) & (0x7f7f7f7f);      
  }

  /******************************************************************************
   * void DepthTest::get_pixel (int *const&s, int i, int x, int y)
   *
   *   Description omitted.  Students might be tempted to copy this
   * function.  Don't.  It doesn't work entirely right for your purposes.
   *****************************************************************************/
  int DepthTest::get_pixel (int *const&s, int i, int x, int y)
  {
    int offset = ((i % image_count * 424 + ((y&0x7fffffff) < 0x1a8 ? y : 0))<<9) + ((x & (~0x1ff)) == 0 ? x : 0);
    
    // Obscure code to avoid copying.

    return s[offset];
  }

  /* End of DepthTest class definitions */

  /******************************************************************************
   * int main (int argc, char** argv)
   *
   *   Blob detection testing application - runs all tests and reports results.
   *****************************************************************************/
  int main (int argc, char** argv)
  {
    // Parse the command line arguments.
    
    Parameters parms (argc, argv);

    if (parms.error)
      return -1;

    if (parms.verbose)
    {
      cout << "Verbose mode active." << endl;
      cout << "Seed:      " << parms.seed << endl;
      cout << "Tests:     " << parms.test_count << endl;
      cout << "Batch size " << parms.batch_size << endl;
    }

    // Test loop

    double total_elapsed_time = 0;

    for (int test = 1; test <= parms.test_count; test++)
    {
      if (parms.verbose)
	cout << "Test " << test << ":" << endl;
      
      // Generate test data.

      DepthTest test_data(parms, test);

      // Run the test.

      if (parms.verbose)
	cout << "  Starting blob detection..." << endl;      

      double start_ticks = (double) clock();
      mark_blobs (test_data.results, test_data.blob_data, test_data.image_count, test_data.near_threshold, test_data.far_threshold); ////////////////////////////////////////////////////////////////////////////////////////////////////////
      double elapsed_time = ((double) clock() - start_ticks) / CLOCKS_PER_SEC;
      total_elapsed_time += elapsed_time;

      if (parms.verbose)
      {
	cout << "  Blob detection done." << endl;
	cout << "  Elapsed time: " << fixed << setprecision(4) << elapsed_time << endl;
      }

      // Validate results.

    bool succeeded = test_data.validate();

      if (parms.verbose)
	cout << (succeeded ? "  Test passed." : "Test failed.") << endl;
      
      // Bail if tests failed.

      if (!succeeded)
      {
        cout << "Fail - at least one test failed." << endl;
    	return -1;
      }
    }

    // Since errors abort the application, if we get here everything passed.

    double average_millis = total_elapsed_time / (parms.test_count * parms.batch_size);
    
    cout << "Success - all tests passed." << endl;
    cout << "Average milliseconds per image: " << fixed << setprecision(4) << average_millis << endl;
    if (parms.batch_size < 100)
      cout << "(Note:  Less than 100 images were tested in each batch.  The average may not be very accurate.)" << endl;
  }

  /* End of cs_3505::main */
  
  /* End of cs_3505 namespace */



// Forward declaration - this function should be in a separate file
  //   supplied by the student.  This separate file is the student's sumbission.
  //   (Students must not put their blob detector in this file.)
  
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
    for (int i = 0 ; i < image_count * 512 *424; i ++){
      
      


    }//close i for loop


  }//close function mark_blobs 


  
}


/* Application entry point */

/******************************************************************************
 * int main (int argc, char** argv)
 *
 *   Application entry point.  Simply calls the application main that is in
 * the cs_3505 namespace.
 *****************************************************************************/
int main (int argc, char** argv)
{
  return cs_3505::main (argc, argv);
}

