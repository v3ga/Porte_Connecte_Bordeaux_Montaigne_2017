// ------------------------------------
import java.util.Timer;
import java.util.TimerTask;
import java.util.List;
import twitter4j.conf.*;
import twitter4j.*;
import twitter4j.auth.*;
import twitter4j.api.*;
import java.io.IOException;
import java.util.regex.*;



// ------------------------------------
// Timer
Timer timerLoadData;
// Période
float periodLoadData = 30.0;
// Objet twitter
Twitter twitter;
// Tweets
ArrayList<Status> tweets = null;
Status tweetCurrent = null;
// Recherche tweets
long twitLastId = 0;
boolean twitLoading = false;
boolean twitSaying = false;

// ------------------------------------------------
String recherche = "#poutou";
float periodSay = 10.0; // en secondes, délai entre chaque lecture de tweets
// ------------------------------------------------

// Debug
boolean __DEBUG__ = true;

// ------------------------------------
void setup()
{
  ConfigurationBuilder cb = new ConfigurationBuilder();
  cb.setOAuthConsumerKey("ATkryfXkP1q348Rsx4ynF5reD");
  cb.setOAuthConsumerSecret("aVBkbgMHODcYseDWeXGSKDJuVS5btb17QRhZRvMCHvzUdbgNke");
  cb.setOAuthAccessToken("8675642-eyP2M9ZPgJR4ozsz3XWeYqVHhhM1e2wxFjopPIenj7");
  cb.setOAuthAccessTokenSecret("QV4vsnK4mQE35DcZaggLY6r0t91Sijpv8p1cbeLbiYcou");

  TwitterFactory tf = new TwitterFactory(cb.build());
  twitter = tf.getInstance();

  tweets = new ArrayList<Status>();

  timerLoadData = new Timer(false);
  timerLoadData.scheduleAtFixedRate(new DataLoadTask(), 0, (long) periodLoadData*1000);

  (new Timer(false)).scheduleAtFixedRate(new SayTask(), 0, (long) periodSay*1000);


  size(500, 500);
}

// ------------------------------------
void draw()
{
  background(0);
  if (twitLoading == false && tweets.size()>0 && twitSaying)
  {
    tweetCurrent = tweets.remove(0);
    TextToSpeech.say(removeUrl(tweetCurrent.getText()), "Amelie", 150);

    twitSaying = false;
  }
  
  if (tweetCurrent != null)
  {
    fill(255);
    text(tweetCurrent.getText()+"\n nb retweets="+tweetCurrent.getRetweetCount(), 10,10, width-10, height-10);
    text("nb tweets="+tweets.size(), 10, height-10);
  }
    
}

// ------------------------------------
void loadData()
{
  twitLoading = true;
  try
  {
    List<Status> newTweets = null;

    Query query = new Query(recherche);
    if (twitLastId > 0)
      query.setSinceId(twitLastId);
    QueryResult result = twitter.search(query);
    newTweets = result.getTweets();

    for (Status s : newTweets)
    {
      tweets.add( s );
      if (s.getId() > twitLastId)
        twitLastId = s.getId();
    }
  }
  catch(TwitterException e)
  {
  }
  twitLoading = false;
}

// ------------------------------------
class DataLoadTask extends TimerTask
{
  public void run() 
  {
    loadData();
  }
}

// ------------------------------------
class SayTask extends TimerTask
{
  public void run() 
  {
    twitSaying = true;
  }
}


// ------------------------------------
void mousePressed()
{
//  TextToSpeech.say("Salut les gars !", "Amelie", 200);

}