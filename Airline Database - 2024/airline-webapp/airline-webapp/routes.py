# Importing the Flask Framework

from flask import *
import database
import configparser
from pg8000.dbapi import *


# appsetup

page = {}
session = {}

# Initialise the FLASK application
app = Flask(__name__)
app.secret_key = 'SoMeSeCrEtKeYhErE'


# Debug = true if you want debug output on error ; change to false if you dont
app.debug = True


# Read my unikey to show me a personalised app
config = configparser.ConfigParser()
config.read('config.ini')
dbuser = config['DATABASE']['user']
portchoice = config['FLASK']['port']
if portchoice == '10000':
    print('ERROR: Please change config.ini as in the comments or Lab instructions')
    exit(0)

session['isadmin'] = False

###########################################################################################
###########################################################################################
####                                 Database operative routes                         ####
###########################################################################################
###########################################################################################



#####################################################
##  INDEX
#####################################################

# What happens when we go to our website (home page)
@app.route('/')
def index():
    # If the user is not logged in, then make them go to the login page
    if( 'logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))
    page['username'] = dbuser
    page['title'] = 'Welcome'
    return render_template('welcome.html', session=session, page=page)

#####################################################
# User Login related                        
#####################################################
# login
@app.route('/login', methods=['POST', 'GET'])
def login():
    page = {'title' : 'Login', 'dbuser' : dbuser}
    # If it's a post method handle it nicely
    if(request.method == 'POST'):
        # Get our login value
        val = database.check_login(request.form['userid'], request.form['password'])
        print(val)
        print(request.form)
        # If our database connection gave back an error
        if(val == None):
            errortext = "Error with the database connection."
            errortext += "Please check your terminal and make sure you updated your INI files."
            flash(errortext)
            return redirect(url_for('login'))

        # If it's null, or nothing came up, flash a message saying error
        # And make them go back to the login screen
        if(val is None or len(val) < 1):
            flash('There was an error logging you in')
            return redirect(url_for('login'))

        # If it was successful, then we can log them in :)
        print(val[0])
        session['name'] = val[0]['firstname']
        session['userid'] = request.form['userid']
        session['logged_in'] = True
        session['isadmin'] = val[0]['isadmin']
        return redirect(url_for('index'))
    else:
        # Else, they're just looking at the page :)
        if('logged_in' in session and session['logged_in'] == True):
            return redirect(url_for('index'))
        return render_template('index.html', page=page)

# logout
@app.route('/logout')
def logout():
    session['logged_in'] = False
    flash('You have been logged out')
    return redirect(url_for('index'))



########################
#List All Items#
########################

@app.route('/users')
def list_users():
    '''
    List all rows in users by calling the relvant database calls and pushing to the appropriate template
    '''
    # connect to the database and call the relevant function
    users_listdict = database.list_users()

    # Handle the null condition
    if (users_listdict is None):
        # Create an empty list and show error message
        users_listdict = []
        flash('Error, there are no rows in users')
    page['title'] = 'List Contents of users'
    return render_template('list_users.html', page=page, session=session, users=users_listdict)
    

########################
#List Single Items#
########################


@app.route('/users/<userid>')
def list_single_users(userid):
    '''
    List all rows in users that match a particular id attribute userid by calling the 
    relevant database calls and pushing to the appropriate template
    '''

    # connect to the database and call the relevant function
    users_listdict = None
    users_listdict = database.list_users_equifilter("userid", userid)

    # Handle the null condition
    if (users_listdict is None or len(users_listdict) == 0):
        # Create an empty list and show error message
        users_listdict = []
        flash('Error, there are no rows in users that match the attribute "userid" for the value '+userid)
    page['title'] = 'List Single userid for users'
    return render_template('list_users.html', page=page, session=session, users=users_listdict)


########################
#List Search Items#
########################

@app.route('/consolidated/users')
def list_consolidated_users():
    '''
    List all rows in users join userroles 
    by calling the relvant database calls and pushing to the appropriate template
    '''
    # connect to the database and call the relevant function
    users_userroles_listdict = database.list_consolidated_users()

    # Handle the null condition
    if (users_userroles_listdict is None):
        # Create an empty list and show error message
        users_userroles_listdict = []
        flash('Error, there are no rows in users_userroles_listdict')
    page['title'] = 'List Contents of Users join Userroles'
    return render_template('list_consolidated_users.html', page=page, session=session, users=users_userroles_listdict)

@app.route('/user_stats')
def list_user_stats():
    '''
    List some user stats
    '''
    # connect to the database and call the relevant function
    user_stats = database.list_user_stats()

    # Handle the null condition
    if (user_stats is None):
        # Create an empty list and show error message
        user_stats = []
        flash('Error, there are no rows in user_stats')
    page['title'] = 'User Stats'
    return render_template('list_user_stats.html', page=page, session=session, users=user_stats)

@app.route('/users/search', methods=['POST', 'GET'])
def search_users_byname():
    '''
    List all rows in users that match a particular name
    by calling the relevant database calls and pushing to the appropriate template
    '''
    if(request.method == 'POST'):

        search = database.search_users_customfilter(request.form['searchfield'],"~",request.form['searchterm'])
        print(search)
        
        users_listdict = None

        if search == None:
            errortext = "Error with the database connection."
            errortext += "Please check your terminal and make sure you updated your INI files."
            flash(errortext)
            return redirect(url_for('index'))
        if search == None or len(search) < 1:
            flash(f"No items found for search: {request.form['searchfield']}, {request.form['searchterm']}")
            return redirect(url_for('index'))
        else:
            
            users_listdict = search
            # Handle the null condition'
            if (users_listdict is None or len(users_listdict) == 0):
                # Create an empty list and show error message
                users_listdict = []
                flash('Error, there are no rows in users that match the searchterm '+request.form['searchterm'])
            page['title'] = 'Users search by name'
            return render_template('list_users.html', page=page, session=session, users=users_listdict)
            

    else:
        return render_template('search_users.html', page=page, session=session)
        
@app.route('/users/delete/<userid>')
def delete_user(userid):
    '''
    Delete a user
    '''
    # connect to the database and call the relevant function
    resultval = database.delete_user(userid)
    
    page['title'] = f'List users after user {userid} has been deleted'
    return redirect(url_for('list_consolidated_users'))
    
@app.route('/users/update', methods=['POST','GET'])
def update_user():
    """
    Update details for a user
    """
    # # Check if the user is logged in, if not: back to login.
    if('logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))
    
    # Need a check for isAdmin

    page['title'] = 'Update user details'

    userslist = None

    print("request form is:")
    newdict = {}
    print(request.form)

    validupdate = False
    # Check your incoming parameters
    if(request.method == 'POST'):

        # verify that at least one value is available:
        if ('userid' not in request.form):
            # should be an exit condition
            flash("Can not update without a userid")
            return redirect(url_for('list_users'))
        else:
            newdict['userid'] = request.form['userid']
            print("We have a value: ",newdict['userid'])

        if ('firstname' not in request.form):
            newdict['firstname'] = None
        else:
            validupdate = True
            newdict['firstname'] = request.form['firstname']
            print("We have a value: ",newdict['firstname'])

        if ('lastname' not in request.form):
            newdict['lastname'] = None
        else:
            validupdate = True
            newdict['lastname'] = request.form['lastname']
            print("We have a value: ",newdict['lastname'])

        if ('userroleid' not in request.form):
            newdict['userroleid'] = None
        else:
            validupdate = True
            newdict['userroleid'] = request.form['userroleid']
            print("We have a value: ",newdict['userroleid'])

        if ('password' not in request.form):
            newdict['password'] = None
        else:
            validupdate = True
            newdict['password'] = request.form['password']
            print("We have a value: ",newdict['password'])

        print('Update dict is:')
        print(newdict, validupdate)

        if validupdate:
            #forward to the database to manage update
            userslist = database.update_single_user(newdict['userid'],newdict['firstname'],newdict['lastname'],newdict['userroleid'],newdict['password'])
        else:
            # no updates
            flash("No updated values for user with userid")
            return redirect(url_for('list_users'))
        # Should redirect to your newly updated user
        return list_single_users(newdict['userid'])
    else:
        return redirect(url_for('list_consolidated_users'))

######
## Edit user
######
@app.route('/users/edit/<userid>', methods=['POST','GET'])
def edit_user(userid):
    """
    Edit a user
    """
    # # Check if the user is logged in, if not: back to login.
    if('logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))
    
    # Need a check for isAdmin

    page['title'] = 'Edit user details'

    users_listdict = None
    users_listdict = database.list_users_equifilter("userid", userid)

    # Handle the null condition
    if (users_listdict is None or len(users_listdict) == 0):
        # Create an empty list and show error message
        users_listdict = []
        flash('Error, there are no rows in users that match the attribute "userid" for the value '+userid)

    userslist = None
    print("request form is:")
    newdict = {}
    print(request.form)
    user = users_listdict[0]
    validupdate = False

    # Check your incoming parameters
    if(request.method == 'POST'):
        print(request.form)


        # verify that at least one value is available:
        if ('userid' not in request.form):
            # should be an exit condition
            flash("Can not update without a userid")
            return redirect(url_for('list_users'))
        else:
            newdict['userid'] = request.form['userid']
            print("We have a value: ",newdict['userid'])

        if ('firstname' not in request.form):
            newdict['firstname'] = None
        else:
            validupdate = True
            newdict['firstname'] = request.form['firstname']
            print("We have a value: ",newdict['firstname'])

        if ('lastname' not in request.form):
            newdict['lastname'] = None
        else:
            validupdate = True
            newdict['lastname'] = request.form['lastname']
            print("We have a value: ",newdict['lastname'])

        if ('userroleid' not in request.form):
            newdict['userroleid'] = None
        else:
            validupdate = True
            newdict['userroleid'] = request.form['userroleid']
            print("We have a value: ",newdict['userroleid'])

        if ('password' not in request.form):
            newdict['password'] = None
        else:
            validupdate = True
            newdict['password'] = request.form['password']
            print("We have a value: ",newdict['password'])

        print('Update dict is:')
        print(newdict, validupdate)

        if validupdate:
            #forward to the database to manage update
            userslist = database.update_single_user(newdict['userid'],newdict['firstname'],newdict['lastname'],newdict['userroleid'],newdict['password'])
        else:
            # no updates
            flash("No updated values for user with userid")
            return redirect(url_for('list_users'))
        # Should redirect to your newly updated user
        return list_single_users(newdict['userid'])
    else:
        # assuming GET request, need to setup for this
        return render_template('edit_user.html',
                           session=session,
                           page=page,
                           userroles=database.list_userroles(),
                           user=user)


######
## add items
######

@app.route('/users/add', methods=['POST','GET'])
def add_user():
    """
    Add a new User
    """
    # # Check if the user is logged in, if not: back to login.
    if('logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))
    
    # Need a check for isAdmin

    page['title'] = 'Add user details'

    userslist = None
    print("request form is:")
    newdict = {}
    print(request.form)

    # Check your incoming parameters
    if(request.method == 'POST'):

        # verify that all values are available:
        if ('userid' not in request.form):
            # should be an exit condition
            flash("Can not add user without a userid")
            return redirect(url_for('add_user'))
        else:
            newdict['userid'] = request.form['userid']
            print("We have a value: ",newdict['userid'])

        if ('firstname' not in request.form):
            newdict['firstname'] = 'Empty firstname'
        else:
            newdict['firstname'] = request.form['firstname']
            print("We have a value: ",newdict['firstname'])

        if ('lastname' not in request.form):
            newdict['lastname'] = 'Empty lastname'
        else:
            newdict['lastname'] = request.form['lastname']
            print("We have a value: ",newdict['lastname'])

        if ('userroleid' not in request.form):
            newdict['userroleid'] = 1 # default is traveler
        else:
            newdict['userroleid'] = request.form['userroleid']
            print("We have a value: ",newdict['userroleid'])

        if ('password' not in request.form):
            newdict['password'] = 'blank'
        else:
            newdict['password'] = request.form['password']
            print("We have a value: ",newdict['password'])

        print('Insert parametesrs are:')
        print(newdict)

        database.add_user_insert(newdict['userid'], newdict['firstname'],newdict['lastname'],newdict['userroleid'],newdict['password'])
        # Should redirect to your newly updated user
        return redirect(url_for('list_consolidated_users'))
    else:
        # assuming GET request, need to setup for this
        return render_template('add_user.html',
                           session=session,
                           page=page,
                           userroles=database.list_userroles())


########################
#AIRPORTS#
########################

# list airports
@app.route('/airports')
def list_airports():
    # connect to the database and call the relevant function
    sort = request.args.get('sort', 'airportid')
    order = request.args.get('order', 'asc')

    if not sort:
        sort = 'airportid'
    if not order:
        order = 'asc'

    page_number = int(request.args.get('page_number', 1))
    limit = 10
    offset = (page_number-1)* limit

    attribute_name = request.args.get('attribute_name')
    attribute_term = request.args.get('attribute_term')

    if (attribute_name == "Airport ID"):
        search = database.search_airports_id(attribute_term)

    else:
        search = database.search_airports_customfilter_order(attribute_name,"~",attribute_term, sort, order, limit, offset)

    airports_listdict = search

    # Handle the null condition
    if (airports_listdict is None):
        # Create an empty list and show error message
        airports_listdict = []
        flash('Error, there are no rows in airports')

    if (attribute_name == "Airport ID"):
        total_airports = 1  #as is a primary key
    else:
        total_airports = database.get_total_airports_count(attribute_name, "~", attribute_term)
    
    total_pages = ((total_airports[0])['count'] + limit - 1) // limit

    page['title'] = 'List Contents of airports'
    return render_template('list_airports.html', session=session, page=page, page_number=page_number, total_pages=total_pages, airports=airports_listdict, 
                           attribute_name=attribute_name, attribute_term=attribute_term)


# delete airport
@app.route('/airports/delete/<airportid>')
def delete_airport(airportid):
    '''
    Delete an airport
    '''
    try:
        # connect to the database and call the relevant function
        resultval = database.delete_airport(airportid)
        
        page['title'] = f'List Airports after airport {airportid} has been deleted'

    except ProgrammingError as e:
        if 'violates foreign key constraint' in str(e):
            flash(f'Error: Airport {airportid} cannot be deleted because it is referenced by another table.', 'danger')
        else:
            flash('An unexpected error occurred.', 'danger')
    return redirect(url_for('list_airports'))


# add airport
@app.route('/airports/add', methods=['POST','GET'])
def add_airport():
    """
    Add a new Airport
    """
    # # Check if the user is logged in, if not: back to login.
    if('logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))

    page['title'] = 'Add airport details'

    newdict = {}

    # Check your incoming parameters
    if(request.method == 'POST'):

        # verify that all values are available:
        if ('airportid' not in request.form):

            flash("Can not add airport without an airportid")
            return redirect(url_for('add_airport'))
        else:
            newdict['airportid'] = request.form['airportid']
            
            try:
                if not isinstance(int(newdict['airportid']), int):
                    flash("Error: Airport ID must be an integer")
                    return redirect(url_for('add_airport'))
            except ValueError:
                flash("Error: Airport ID must be an integer")
                return redirect(url_for('add_airport'))
            
            if database.airport_id_exists(newdict['airportid']):
                flash("AirportID already exists. Please choose another.")
                return redirect(url_for('add_airport'))

        if ('name' not in request.form):
            newdict['name'] = ''
        else:
            newdict['name'] = request.form['name']

        if ('iatacode' not in request.form):
            newdict['iatacode'] = ''
        else:
            print(f"hello: {len(request.form['iatacode'])}")
            if (len(request.form['iatacode']) != 3 and len(request.form['iatacode']) != 0):
                flash("Error: IATA Code must be of length 3 in form: XXX")
                return redirect(url_for('add_airport'))
            else:
                newdict['iatacode'] = request.form['iatacode']

            print("We have a value: ",newdict['iatacode'])

        if ('country' not in request.form):
            newdict['country'] = ''
        else:
            newdict['country'] = request.form['country']

        if ('city' not in request.form):
            newdict['city'] = ''
        else:
            newdict['city'] = request.form['city']
            print("We have a value: ",newdict['city'])

        print('Insert parametesrs are:')
        print(newdict)

        try:
            database.add_airport_insert(newdict['airportid'], newdict['name'],newdict['iatacode'],newdict['country'],newdict['city'])

        except IntegrityError:
            flash("Error: Airport ID already exists. Please choose a different ID.")
        return redirect(url_for('list_airports'))
    else:
        return render_template('add_airport.html',
                           session=session,
                           page=page)


# edit airport
@app.route('/airports/edit/<airportid>', methods=['POST','GET'])
def edit_airport(airportid):
    # # Check if the user is logged in, if not: back to login.
    if('logged_in' not in session or not session['logged_in']):
        return redirect(url_for('login'))

    page['title'] = 'Edit airport details'

    airports_listdict = None
    airports_listdict = database.list_airports_equifilter("airportid", airportid)

    # Handle the null condition
    if (airports_listdict is None or len(airports_listdict) == 0):
        # Create an empty list and show error message
        airports_listdict = []
        flash('Error, there are no rows in airports that match the attribute "airport" for the value '+airport)

    newdict = {}
    airport = airports_listdict[0]
    validupdate = False

    # Check your incoming parameters
    if(request.method == 'POST'):
        if ('airportid' not in request.form):
            flash("Can not update without an airportid")
            return redirect(url_for('list_airports'))
        else:
            if airportid != request.form['airportid']:
                flash("Cannot change airportid")
                return redirect(url_for('edit_airport', airportid=airportid))

        if ('name' not in request.form):
            newdict['name'] = None
        else:
            validupdate = True
            newdict['name'] = request.form['name']

        if ('iatacode' not in request.form):
            newdict['iatacode'] = None
        else:
            if (len(request.form['iatacode']) != 3 and len(request.form['iatacode']) != 0):
                flash("IATA Code must be of length 3 in form: XXX")
                return redirect(url_for('edit_airport', airportid=airportid))
            else:
                validupdate = True
                newdict['iatacode'] = request.form['iatacode']

        if ('city' not in request.form):
            newdict['city'] = None
        else:
            validupdate = True
            newdict['city'] = request.form['city']

        if ('country' not in request.form):
            newdict['country'] = None
        else:
            validupdate = True
            newdict['country'] = request.form['country']

        if validupdate:
            database.update_single_airport(airportid,newdict['name'],newdict['iatacode'],newdict['city'],newdict['country'])
        else:
            # no updates
            flash("No updated values for airport with airportid")
            return redirect(url_for('list_airports'))

        return list_single_airports(airportid)
    else:
        # assuming GET request, need to setup for this
        return render_template('edit_airport.html',
                           session=session,
                           page=page,
                           airport=airport)
    

# list airport by airportid
@app.route('/airports/<airportid>')
def list_single_airports(airportid):

    # connect to the database and call the relevant function
    airports_listdict = None
    airports_listdict = database.list_airports_equifilter("airportid", airportid)

    # Handle the null condition
    if (airports_listdict is None or len(airports_listdict) == 0):
        # Create an empty list and show error message
        airports_listdict = []
        flash('Error, there are no rows in airports that match the attribute "airportid" for the value '+airportid)
    page['title'] = 'List Single airportid for airports'
    return render_template('list_airports.html', page=page, session=session, page_number=1, total_pages=1, airports=airports_listdict)


# list airports by countries
@app.route('/airports/countries')
def list_airports_by_country():
    sort = request.args.get('sort', 'airport_count')
    order = request.args.get('order', 'asc')
    # connect to the database and call the relevant function
    airports_listdict = database.list_airports_by_country(sort, order)

    # Handle the null condition
    if (airports_listdict is None):
        # Create an empty list and show error message
        airports_listdict = []
        flash('There are no countries')

    page['title'] = 'Airports by Countries'
    return render_template('list_airports_by_countries.html', page=page, session=session, airports=airports_listdict)


# search airports
@app.route('/airports/searchbyname', methods=['POST', 'GET'])
def search_airports_byname():

    if(request.method == 'POST'):
        page_number = int(request.args.get('page_number', 1))
        limit = 10
        offset = (page_number-1)* limit
        
        attribute_name = request.form['searchfield']
        attribute_term = request.form['searchterm']

        if (attribute_name == "IATA Code"):
            attribute_name = "iatacode" 

        if (attribute_name == "Airport ID"):
            try:
                if not isinstance(int(attribute_term), int):
                    flash("Error: Airport ID must be an integer")
                    return render_template('search_airports.html', page=page, session=session, airport_columns=database.get_airport_columns())
            except ValueError:
                flash("Error: Airport ID must be an integer")
                return redirect(url_for('add_airport'))
    
            search = database.search_airports_id(attribute_term)      

        elif (attribute_name == "IATA Code"):
            if (len(attribute_term) != 3):
                flash("Error: IATA Code must be of length 3 in form XXX")
                return render_template('search_airports.html', page=page, session=session, airport_columns=database.get_airport_columns())
            else:
                search = database.search_airports_customfilter_order(attribute_name,"~",attribute_term, "airportid", "asc", limit, offset)

        else:    
            search = database.search_airports_customfilter_order(attribute_name,"~",attribute_term, "airportid", "asc", limit, offset)
        
        airports_listdict = None

        if search == None:
            errortext = "Error with the database connection."
            errortext += "Please check your terminal and make sure you updated your INI files."
            flash(errortext)
            return redirect(url_for('index'))
        if search == None or len(search) < 1:
            flash(f"No items found for search: {attribute_name}, {attribute_term}")
            return redirect(url_for('index'))
        else:
            airports_listdict = search
            # Handle the null condition'
            if (airports_listdict is None or len(airports_listdict) == 0):
                # Create an empty list and show error message
                airports_listdict = []
                flash('Error, there are no rows in airports that match the searchterm '+ attribute_term)
            
            print(f"airports list: {airports_listdict}")
            if (attribute_name == "Airport ID"):
                total_airports = 1  #as is a primary key
                total_pages = 1
            else:
                total_airports = database.get_total_airports_count(attribute_name, "~", attribute_term)
                print(f"TOTAL AIRPORTS: {total_airports}")
                total_pages = ((total_airports[0])['count'] + limit - 1) // limit

            page['title'] = f'Airports search for {attribute_name} = {attribute_term}'
            return render_template('list_airports.html', page=page, session=session, page_number=page_number, total_pages=total_pages, airports=airports_listdict, 
                                   attribute_name=attribute_name, attribute_term=attribute_term)

    else:
        return render_template('search_airports.html', page=page, session=session, airport_columns=database.get_airport_columns())
    

# list airports in particular country
@app.route('/airports/displaycountries', methods=['POST', 'GET'])
def display_countries():


    sort = request.args.get('sort', 'airportid')
    order = request.args.get('order', 'asc')

    # If the request method is POST, retrieve the country from form data
    if request.method == 'POST':
        country = request.form.get('searchterm', 'Australia')  # Default to 'Australia' if not provided
    else:
        country = request.args.get('country', 'Australia')

    search = database.search_airports_customfilter_order("country","~", country, sort, order, None, None)
    airports_listdict = search

    if (search is None):
        # Create an empty list and show error message
        search = []
        flash('There are no countries')
    page['title'] = f'Airports in {country}'
    return render_template('list_airports_countries.html', page=page, session=session, airports=airports_listdict, country=country)