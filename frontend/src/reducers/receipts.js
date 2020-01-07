import { GET_RECEIPTS, ADD_RECEIPT, DELETE_RECEIPT } from '../actions/types';

const initialState = {
	receipts : []
}

export default function (state = initialState, action) {
	switch (action.type) {
		case GET_RECEIPTS:
			return {
				...state,
				receipts: action.payload
			}
		case DELETE_RECEIPT:
			return {
				...state,
				receipts: state.receipts.filter(receipt => receipt.id !== action.payload)
			};
		case ADD_RECEIPT:
			return {
				...state,
				receipts: [...state.receipts, action.payload]
			}
		default:
			return state;
	}
}