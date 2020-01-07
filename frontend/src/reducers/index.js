import { combineReducers } from 'redux';
import receipts from './receipts';
import errors from './errors';
import messages from './messages';
import auth from './auth';

export default combineReducers({
    receipts,
    errors,
    messages,
    auth
});