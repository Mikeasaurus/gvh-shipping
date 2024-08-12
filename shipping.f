*********************************************************************
*                                                                   *
*  ANTHROPOMORPHIC DINOSAUR SHIP SIMULATOR                          *
*                                                                   *
*  (C) 1979 UNIVERSITY OF EDUCATION CORPORATION                     *
*      DEPARTMENT OF PALEO RELATIONS                                *
*                                                                   *
*  FUNDED AS PART OF PROJECT POLYCULE                               *
*  FOR INTERNAL USE ONLY ON DESIGNATED MAINFRAME SYSTEMS            *
*                                                                   *
*  PLEASE KEEP ALL PUNCHCARDS IN ORDER!                             *
*  RETURN FULL BOX TO ROOM 103 WHEN FINISHED TESTING                *
*  FULL DOCUMENTATION CAN BE FOUND ON LOWER SHELF, RIGHTHAND SIDE   *
*                                                                   *
*********************************************************************

      RECURSIVE SUBROUTINE TRAVERSE (G, VISITED, I, N)
      IMPLICIT NONE
      LOGICAL G(N,N)
      LOGICAL VISITED(N)
      INTEGER I, J, JM, N
      VISITED(I) = .TRUE.
      DO J = I+1, I+N
      JM = J
      IF (JM .GT. N) JM = J - N
      IF (G(I,JM) .AND. .NOT. VISITED(JM)) THEN
      CALL TRAVERSE (G, VISITED, JM, N)
      END IF
      END DO
      END SUBROUTINE TRAVERSE

      FUNCTION ISVALID (G,N)
      IMPLICIT NONE
      LOGICAL ISVALID, G(N,N), PARTICIPANT(N), VISITED(N)
      INTEGER N, I, J
      ISVALID = .FALSE.
      DO I = 1, N
      PARTICIPANT(I) = .FALSE.
      DO J = 1, N
      IF (G(I,J)) PARTICIPANT(I) = .TRUE.
      END DO
      END DO
      IF (PARTICIPANT(1) .AND. PARTICIPANT(2)) GOTO 210
      DO I = 1, N
      VISITED(I) = .FALSE.
      END DO
      I = 0
200   I = I + 1
      IF (.NOT. PARTICIPANT(I)) GOTO 200
      CALL TRAVERSE (G, VISITED, I, N)
      DO I = 1, N
      IF (PARTICIPANT(I) .AND. .NOT. VISITED(I)) GOTO 210
      END DO
      ISVALID = .TRUE.
210   CONTINUE
      END FUNCTION ISVALID

      PROGRAM SHIPPING
      IMPLICIT NONE
      INTEGER N, N2, EN2, C, IND, TMP, I, J
      PARAMETER (N = 8)
      PARAMETER (N2 = N*(N-1)/2)
      PARAMETER (EN2 = ISHFT(1,N2))
      LOGICAL G(N,N), ISVALID
      C = 0
      DO IND = 1, EN2-1
      TMP = IND
      I = 0
310   I = I + 1
      IF (I .GT. N) GOTO 330
      G(I,I) = .FALSE.
      J = I
320   J = J + 1
      IF (J .GT. N) GOTO 310
      G(I,J) = (MOD(TMP,2).EQ.1)
      G(J,I) = G(I,J)
      TMP = ISHFT(TMP,-1)
      GOTO 320
330   CONTINUE
      IF (ISVALID(G,N)) C = C + 1
      END DO
      WRITE (*,*) "TOTAL COUNT:", C
      END PROGRAM SHIPPING
