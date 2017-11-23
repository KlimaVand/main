<%
'--------------------------------------------------------------------
' Microsoft ADO
'
' (c) 1996 Microsoft Corporation.  All Rights Reserved.
'
'
'
' ADO constants include file for VBScript
'
'--------------------------------------------------------------------
'CursorTypeEnum Values
Dim adOpenForwardOnly, adOpenKeyset, adOpenDynamic, adOpenStatic
'CursorOptionEnum Values
Dim adHoldRecords, adMovePrevious, adAddNew, adDelete, adUpdate, adBookmark, adApproxPosition, adUpdateBatch, adResync
'LockTypeEnum Values
Dim adLockReadOnly, adLockPessimistic, adLockOptimistic, adLockBatchOptimistic
'CursorLocationEnum Values
Dim adUseClient, adUseServer, adUseClientBatch
'DataTypeEnum Values
Dim adEmpty, adTinyInt, adSmallInt, adInteger, adBigInt, adUnsignedTinyInt, adUnsignedSmallInt, adUnsignedInt
Dim adUnsignedBigInt, adSingle, adDouble, adCurrency, adDecimal, adNumeric, adBoolean, adError, adUserDefined
Dim adVariant, adIDispatch, adIUnknown, adGUID, adDate, adDBDate, adDBTime, adDBTimeStamp, adBSTR, adChar, adVarChar
Dim adLongVarChar, adWChar, adVarWChar, adLongVarWChar, adBinary, adVarBinary, adLongVarBinary 
'ConnectPromptEnum Values
Dim adPromptAlways, adPromptComplete, adPromptCompleteRequired, adPromptNever
'ConnectModeEnum Values
Dim adModeUnknown, adModeRead, adModeWrite, adModeReadWrite, adModeShareDenyRead, adModeShareDenyWrite
Dim adModeShareExclusive, adModeShareDenyNone
'IsolationLevelEnum Values
Dim adXactUnspecified, adXactChaos, adXactReadUncommitted, adXactBrowse, adXactCursorStability, adXactReadCommitted
Dim adXactRepeatableRead, adXactSerializable, adXactIsolated
'XactAttributeEnum Values
Dim adXactPollAsync, adXactPollSyncPhaseOne, adXactCommitRetaining, adXactAbortRetaining, adXactAbortAsync
'FieldAttributeEnum Values
Dim adFldBookmark, adFldMayDefer, adFldUpdatable, adFldUnknownUpdatable, adFldFixed, adFldIsNullable, adFldMayBeNull
Dim adFldLong, adFldRowID, adFldRowVersion, adFldCacheDeferred
'EditModeEnum Values
Dim adEditNone, adEditInProgress, adEditAdd
'RecordStatusEnum Values
Dim adRecOK, adRecNew, adRecModified, adRecDeleted, adRecUnmodified, adRecInvalid, adRecMultipleChanges 
Dim adRecPendingChanges, adRecCanceled, adRecCantRelease, adRecConcurrencyViolation, adRecIntegrityViolation
Dim adRecMaxChangesExceeded, adRecObjectOpen, adRecOutOfMemory, adRecPermissionDenied, adRecSchemaViolation, adRecDBDeleted
'GetRowsOptionEnum Values
Dim adGetRowsRest
'PositionEnum Values
Dim adPosUnknown, adPosBOF, adPosEOF
'AffectEnum Values
Dim adAffectCurrent, adAffectGroup, adAffectAll
'FilterGroupEnum Values
Dim adFilterNone, adFilterPendingRecords, adFilterAffectedRecords, adFilterFetchedRecords
'PropertyAttributesEnum Values
Dim adPropNotSupported, adPropRequired, adPropOptional, adPropRead, adPropWrite
'ErrorValueEnum Values
Dim adErrInvalidArgument, adErrNoCurrentRecord, adErrIllegalOperation, adErrInTransaction, adErrFeatureNotAvailable
Dim adErrItemNotFound, adErrObjectNotSet, adErrDataConversion, adErrObjectClosed, adErrObjectOpen, adErrProviderNotFound, adErrBoundToCommand
'ParameterAttributesEnum Values
Dim adParamSigned, adParamNullable, adParamLong
'ParameterDirectionEnum Values
Dim adParamUnknown, adParamInput, adParamOutput, adParamInputOutput, adParamReturnValue
'CommandTypeEnum Values
Dim adCmdUnknown, adCmdText, adCmdTable, adCmdStoredProc

'---- CursorTypeEnum Values ----
adOpenForwardOnly = 0
adOpenKeyset = 1
adOpenDynamic = 2
adOpenStatic = 3

'---- CursorOptionEnum Values ----
adHoldRecords = &H00000100
adMovePrevious = &H00000200
adAddNew = &H01000400
adDelete = &H01000800
adUpdate = &H01008000
adBookmark = &H00002000
adApproxPosition = &H00004000
adUpdateBatch = &H00010000
adResync = &H00020000

'---- LockTypeEnum Values ----
adLockReadOnly = 1
adLockPessimistic = 2
adLockOptimistic = 3
adLockBatchOptimistic = 4

'---- CursorLocationEnum Values ----
adUseClient = 1
adUseServer = 2
adUseClientBatch = 3

'---- DataTypeEnum Values ----
adEmpty = 0
adTinyInt = 16
adSmallInt = 2
adInteger = 3
adBigInt = 20
adUnsignedTinyInt = 17
adUnsignedSmallInt = 18
adUnsignedInt = 19
adUnsignedBigInt = 21
adSingle = 4
adDouble = 5
adCurrency = 6
adDecimal = 14
adNumeric = 131
adBoolean = 11
adError = 10
adUserDefined = 132
adVariant = 12
adIDispatch = 9
adIUnknown = 13
adGUID = 72
adDate = 7
adDBDate = 133
adDBTime = 134
adDBTimeStamp = 135
adBSTR = 8
adChar = 129
adVarChar = 200
adLongVarChar = 201
adWChar = 130
adVarWChar = 202
adLongVarWChar = 203
adBinary = 128
adVarBinary = 204
adLongVarBinary = 205

'---- ConnectPromptEnum Values ----
adPromptAlways = 1
adPromptComplete = 2
adPromptCompleteRequired = 3
adPromptNever = 4

'---- ConnectModeEnum Values ----
adModeUnknown = 0
adModeRead = 1
adModeWrite = 2
adModeReadWrite = 3
adModeShareDenyRead = 4
adModeShareDenyWrite = 8
adModeShareExclusive = &Hc
adModeShareDenyNone = &H10

'---- IsolationLevelEnum Values ----
adXactUnspecified = &Hffffffff
adXactChaos = &H00000010
adXactReadUncommitted = &H00000100
adXactBrowse = &H00000100
adXactCursorStability = &H00001000
adXactReadCommitted = &H00001000
adXactRepeatableRead = &H00010000
adXactSerializable = &H00100000
adXactIsolated = &H00100000

'---- XactAttributeEnum Values ----
adXactPollAsync = 2
adXactPollSyncPhaseOne = 4
adXactCommitRetaining = &H00020000
adXactAbortRetaining = &H00040000
adXactAbortAsync = &H00080000

'---- FieldAttributeEnum Values ----
adFldBookmark = &H00000001
adFldMayDefer = &H00000002
adFldUpdatable = &H00000004
adFldUnknownUpdatable = &H00000008
adFldFixed = &H00000010
adFldIsNullable = &H00000020
adFldMayBeNull = &H00000040
adFldLong = &H00000080
adFldRowID = &H00000100
adFldRowVersion = &H00000200
adFldCacheDeferred = &H00001000

'---- EditModeEnum Values ----
adEditNone = &H0000
adEditInProgress = &H0001
adEditAdd = &H0002

'---- RecordStatusEnum Values ----
adRecOK = &H0000000
adRecNew = &H0000001
adRecModified = &H0000002
adRecDeleted = &H0000004
adRecUnmodified = &H0000008
adRecInvalid = &H0000010
adRecMultipleChanges = &H0000040
adRecPendingChanges = &H0000080
adRecCanceled = &H0000100
adRecCantRelease = &H0000400
adRecConcurrencyViolation = &H0000800
adRecIntegrityViolation = &H0001000
adRecMaxChangesExceeded = &H0002000
adRecObjectOpen = &H0004000
adRecOutOfMemory = &H0008000
adRecPermissionDenied = &H0010000
adRecSchemaViolation = &H0020000
adRecDBDeleted = &H0040000

'---- GetRowsOptionEnum Values ----
adGetRowsRest = -1

'---- PositionEnum Values ----
adPosUnknown = -1
adPosBOF = -2
adPosEOF = -3

'---- AffectEnum Values ----
adAffectCurrent = 1
adAffectGroup = 2
adAffectAll = 3

'---- FilterGroupEnum Values ----
adFilterNone = 0
adFilterPendingRecords = 1
adFilterAffectedRecords = 2
adFilterFetchedRecords = 3

'---- PropertyAttributesEnum Values ----
adPropNotSupported = &H0000
adPropRequired = &H0001
adPropOptional = &H0002
adPropRead = &H0200
adPropWrite = &H0400

'---- ErrorValueEnum Values ----
adErrInvalidArgument = &Hbb9
adErrNoCurrentRecord = &Hbcd
adErrIllegalOperation = &Hc93
adErrInTransaction = &Hcae
adErrFeatureNotAvailable = &Hcb3
adErrItemNotFound = &Hcc1
adErrObjectNotSet = &Hd5c
adErrDataConversion = &Hd5d
adErrObjectClosed = &He78
adErrObjectOpen = &He79
adErrProviderNotFound = &He7a
adErrBoundToCommand = &He7b

'---- ParameterAttributesEnum Values ----
adParamSigned = &H0010
adParamNullable = &H0040
adParamLong = &H0080

'---- ParameterDirectionEnum Values ----
adParamUnknown = &H0000
adParamInput = &H0001
adParamOutput = &H0002
adParamInputOutput = &H0003
adParamReturnValue = &H0004

'---- CommandTypeEnum Values ----
adCmdUnknown = 0
adCmdText = &H0001
adCmdTable = &H0002
adCmdStoredProc = &H0004
%>
