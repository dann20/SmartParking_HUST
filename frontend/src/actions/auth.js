import axios from 'axios';
import { returnErrors } from './messages';
import { USER_LOADING, USER_LOADED, AUTH_ERROR, LOGIN_SUCCESS, LOGOUT_SUCCESS, REGISTER_FAIL, REGISTER_SUCCESS } from './types';

//CHECK_TOKEN & LOAD_USER
export const loadUser = () => (dispatch, getState) => {
    //User loading
    dispatch({ type: USER_LOADING });

    //Get token from state
    const token = getState().auth.token;

    //Headers
    const config = {
        headers: {
            'Content-Type': 'application/json'
        }
    }

    //If token, add to headers config
    if (token) {
        config.headers['Authorization'] = `Token ${token}`;
    }


    axios.get('/api/auth/user', config).then(res => {
        dispatch({
            type: USER_LOADED,
            payload: res.data
        });
    }).catch(err => {
        dispatch(returnErrors(err.response.data, err.response.status));
        dispatch({
            type: AUTH_ERROR
        });
    });
}

//LOGIN USER
export const login = (username, password) => dispatch => {

    //Headers
    const config = {
        headers: {
            'Content-Type': 'application/json'
        }
    }

    // Request body
    const body = JSON.stringify({ username, password });
    axios.post('/api/auth/login', body, config).then(res => {
        dispatch({
            type: LOGIN_SUCCESS,
            payload: res.data
        });
    }).catch(err => {
        dispatch(returnErrors(err.response.data, err.response.status));
        dispatch({
            type: LOGIN_FAIL
        })
    })
}

//LOGGOUT USER
export const logout = () => (dispatch, getState) => {


    //Get token from state
    const token = getState().auth.token;

    //Headers
    const config = {
        headers: {
            'Content-Type': 'application/json'
        }
    }

    //If token, add to headers config
    if (token) {
        config.headers['Authorization'] = `Token ${token}`;
    }


    axios.post('/api/auth/logout/', null, config).then(res => {
        dispatch({
            type: LOGOUT_SUCCESS,
        });
    }).catch(err => {
        dispatch(returnErrors(err.response.data, err.response.status));
    });
}

// REGISTER USER
export const register = ({ username, password, email }) => dispatch => {
    // Headers
    const config = {
        headers: {
            "Content-Type": "application/json"
        }
    };

    // Request Body
    const body = JSON.stringify({ username, email, password });

    axios
        .post("/api/auth/register/", body, config)
        .then(res => {
            dispatch({
                type: REGISTER_SUCCESS,
                payload: res.data
            });
        })
        .catch(err => {
            dispatch(returnErrors(err.response.data, err.response.status));
            dispatch({
                type: REGISTER_FAIL
            });
        });
};


// Setup config with token - helper function
export const tokenConfig = getState => {
    // Get token from state
    const token = getState().auth.token;

    // Headers
    const config = {
        headers: {
            "Content-Type": "application/json"
        }
    };

    // If token, add to headers config
    if (token) {
        config.headers["Authorization"] = `Token ${token}`;
    }

    return config;
};
