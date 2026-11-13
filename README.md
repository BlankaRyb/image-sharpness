# image-sharpness
My Contribution

I was responsible for designing and implementing the sharpness evaluation method.

The original task was to assess the quality of scanned grayscale documents. I proposed using Sobel edge detection instead of simply measuring grayscale intensity changes.

My contribution included:

Selecting and adapting the edge detection approach for scanned document evaluation.

Designing a threshold-based evaluation method to distinguish significant edges from minor intensity variations.

Defining how detected edge magnitudes contribute to the final sharpness score.

Implementing DPI-dependent threshold adjustment.

Creating a binary edge map to visualize detected edges and make differences between sharp and blurred scans easier to inspect.

The method was specifically designed for scanned documents, rather than general-purpose photographic sharpness assessment.
