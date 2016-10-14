// $Id$

  // Run the following code when the DOM has been fully loaded.
  jQuery(document).ready(function () {
    // Attach some code to the click event for the
    // link with class "plusone-link".
    jQuery('a.plusone-link').click(function () {
      // When clicked, first define an anonymous function
      // to the variable voteSaved.
      var voteSaved = function (data) {
        // Update the number of votes.
        jQuery('div.score').html(data.total_votes);
        // Update the "Vote" string to "You voted".
        jQuery('div.vote').html(data.voted);
      }
      // Make the AJAX call; if successful the
      // anonymous function in voteSaved is run.
      jQuery.ajax({
        type: 'POST', // Use the POST method.
        url: this.href,
        dataType: 'json',
        success: voteSaved,
        data: 'js=1' // Pass a key/value pair.
      });
      // Prevent the browser from handling the click.
      return false;
    });
  });

