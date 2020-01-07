import axios from 'axios';
import { createMessage, returnErrors } from './messages';
import { GET_RECEIPTS, GET_ERRORS, ADD_RECEIPT, DELETE_RECEIPT } from './types';
import { tokenConfig } from "./auth";

// GET_RECEIPTS
export const getReceipts = () => (dispatch, getState) => {
    axios.get('/api/receipts/', tokenConfig(getState)).then(res => {
        dispatch({
            type: GET_RECEIPTS,
            payload: res.data
        });
    }).catch(err => console.log(err.response.data));
}

// DELETE RECEIPT
export const deleteReceipt = id => (dispatch, getState) => {
    axios
        .delete(`/api/receipts/${id}/`, tokenConfig(getState))
        .then(res => {
            dispatch(createMessage({ deleteReceipt: "Receipt Deleted" }));
            dispatch({
                type: DELETE_RECEIPT,
                payload: id
            });
        })
        .catch(err => console.log(err));
};

//ADD_RECEIPTS
export const addReceipt = (receipt) => (dispatch, getState) => {
    axios.post('/api/receipts/', receipt, tokenConfig(getState)).then(res => {
        dispatch(createMessage({ addReceipt : "Receipt Added" }))
        dispatch({
            type: ADD_RECEIPT,
            payload: res.data
        });
    }).catch(err => dispatch(returnErrors(err.response.data, err.response.status)));
}