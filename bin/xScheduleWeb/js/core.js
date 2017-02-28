$(document).ready(function() {

  window.setInterval(function() {

    if (playingStatus != 'disabled') {
      updateStatus();
    }
    updateNavStatus();
  }, 1000);

  navLoadPlaylists();
  navLoadPlugins();
  checkLogInStatus();


  // if mobile
  if (/Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent)) {
    $('#toggleButtonContainer').attr('class',
      'col-md-5 text-center nav-buttons');
    $('#nutcrackerLogo').attr('height',
      '30px');

  }

});
//global status
var playingStatus =
  `{status: "unknown"}`;

function updateStatus() {
  $.ajax({
    url: '/xScheduleQuery?Query=GetPlayingStatus',
    dataType: "json",
    success: function(response) {
      playingStatus = response;
    }
  });
}

function checkLogInStatus() {
  if (playingStatus.status == 'unknown') {
    $.ajax({
      url: '/xScheduleQuery?Query=GetPlayingStatus',
      dataType: "json",
      success: function(response) {
        if (response.result == "not logged in") {
          window.location.href = "login.html";
        }
      }

    });
  } else if (playingStatus.result == "not logged in") {
    window.location.href = "login.html";
  }


}


function navLoadPlaylists() {
  $.ajax({
    url: '/xScheduleQuery?Query=GetPlayLists',
    dataType: "json",
    success: function(response) {
      for (var i = 0; i < response.playlists.length; i++) {
        var navPlaylists =
          `<li><a onclick="updatePage('page', 'playlists','` +
          response.playlists[
            i].name + `')">` + response.playlists[i].name +
          `</a></li>`;
        $("#playlists ul").append(navPlaylists);
      }
    }
  });
}

function navLoadPlugins() {
  $.ajax({
    url: '/xScheduleQuery?Query=ListWebFolders&Parameters=Plugins',
    dataType: "json",
    success: function(response) {
      for (var i = 0; i < response.folders.length; i++) {
        var navPlugins =
          `<li><a onclick="updatePage('plugin', '` + response.folders[
            i] +
          `')">` + response.folders[i] + `</a></li>`;
        $("#plugins ul").append(navPlugins);
      }
    }
  });
}

function logout() {
  $.ajax({
    url: '/xScheduleLogin?Credential=logout',
    dataType: "json",
    success: function(response) {},
    error: function(error) {
      console.log("ERROR: " + error);
    }
  });
}

function runCommand(name, param) {
  if (param == undefined) {
    $.ajax({
      url: '/xScheduleCommand?Command=' + name,
      error: function(response) {
        jAlert('This is a custom alert box', 'Alert Dialog');
      }
    });
  } else {
    $.ajax({
      url: '/xScheduleCommand?Command=' + name + '&Parameters=' +
        param,
      success: function(response) {}
    });
  }
}

function findPercent(length, left) {
  var secLength = parseInt(length.split(":")[0]) * 60 + parseInt(length
    .split(
      ":")[1]);
  var secLeft = parseInt(left.split(":")[0]) * 60 + parseInt(left.split(
    ":")[1]);
  var percent = (secLength - secLeft) / secLength * 100;
  return Math.round(percent) + "%";
}

function updateNavStatus() {
  checkLogInStatus();

  if (playingStatus.status == 'idle') {

    $('#random').attr('class',
      "btn btn-default glyphicon glyphicon-random disabled"
    );
    $('#steplooping').attr('class',
      "btn btn-default glyphicon glyphicon-repeat disabled"
    );
    $('#playlistlooping').attr('class',
      "btn btn-default glyphicon glyphicon-refresh disabled"
    );
  } else {

    if (playingStatus['random'] == "false") {
      $('#random').attr('class',
        "btn btn-default glyphicon glyphicon-random");
    } else if (playingStatus['random'] == "true") {
      $('#random').attr('class',
        "btn btn-info glyphicon glyphicon-random");
    }
    if (playingStatus['steplooping'] == "false") {
      $('#steplooping').attr('class',
        "btn btn-default glyphicon glyphicon-repeat");
    } else if (playingStatus['steplooping'] == "true") {
      $('#steplooping').attr('class',
        "btn btn-info glyphicon glyphicon-repeat");
    }
    if (playingStatus['playlistlooping'] == "false") {
      $('#playlistlooping').attr('class',
        "btn btn-default glyphicon glyphicon-refresh");
    } else if (playingStatus['playlistlooping'] == "true") {
      $('#playlistlooping').attr('class',
        "btn btn-info glyphicon glyphicon-refresh");
    }
  }
  //output to lights
  if (playingStatus['outputtolights'] == 'false') {
    $('#outputtolights').attr('class',
      "btn btn-danger glyphicon glyphicon-eye-close");
  } else if (playingStatus['outputtolights'] == 'true') {
    $('#outputtolights').attr('class',
      "btn btn-success glyphicon glyphicon-eye-open");
  }

}